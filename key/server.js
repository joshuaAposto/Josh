const express = require('express');
const mongoose = require('mongoose');
const axios = require('axios');
const { v4: uuidv4 } = require('uuid');
const crypto = require('crypto');
const path = require('path');
const cors = require('cors');
const rateLimit = require('express-rate-limit');

const app = express();
const port = process.env.PORT || 3000;

// ==========================================
// Configuration & Secrets
// ==========================================
const LOOT_LINK_API_TOKEN = "ba72281d3b361f5c42095b784e1559941d5730982a47c6701f122707afe57234";
const LOOT_LINK_BASE_URL  = "https://lootdest.org/s?tEV6Qcgt";
const YOUR_DOMAIN         = "https://bskey.vercel.app";
const SERVER_SECRET       = "SJSIDIIDJEJRKRKRKKRDIIDIDKDKDKDKFKTJTJJFJFJJFFKFKKFKFKFKFIDIR";
const ADMIN_PASSWORD      = "joshua";
const mongoURI            = "mongodb+srv://ryukodevv:09090577409@cluster0.6ophm4c.mongodb.net/?appName=Cluster0";

// ==========================================
// Lazy MongoDB connection (Vercel serverless safe)
// ==========================================
let cachedConn = null;

async function dbConnect() {
    if (cachedConn && mongoose.connection.readyState === 1) return cachedConn;
    cachedConn = await mongoose.connect(mongoURI, {
        serverSelectionTimeoutMS: 10000,
        socketTimeoutMS: 45000,
    });
    console.log('MongoDB connected');
    return cachedConn;
}

// ==========================================
// Schemas & Models
// ==========================================
const sessionSchema = new mongoose.Schema({
    sessionId: { type: String, required: true, unique: true },
    hwid:      { type: String, required: true },
    userIp:    { type: String },
    createdAt: { type: Number, required: true }
});

const keySchema = new mongoose.Schema({
    keyString:  { type: String, required: true, unique: true },
    hwid:       { type: String, required: true, unique: true },
    ipAddress:  { type: String, required: true },
    expiresAt:  { type: Number, required: true }
});

const Session = mongoose.models.Session || mongoose.model('Session', sessionSchema);
const Key     = mongoose.models.Key     || mongoose.model('Key',     keySchema);

// ==========================================
// Middlewares
// ==========================================
app.use(cors());
app.set('trust proxy', 1);

const generateLimiter = rateLimit({
    windowMs: 15 * 60 * 1000,
    max: 10,
    message: 'Too many requests. Please try again in 15 minutes.'
});

const verifySignature = (req, res, next) => {
    const { hwid, signature } = req.query;
    if (!hwid || !signature) return res.status(403).send("Missing Security Parameters.");
    const expectedHash = crypto.createHash('md5').update('hwid=' + hwid + SERVER_SECRET).digest('hex');
    if (signature !== expectedHash) return res.status(403).send("Bypass Attempt Detected.");
    next();
};

function getClientIp(req) {
    const forwarded = req.headers['x-forwarded-for'];
    return forwarded ? forwarded.split(',')[0].trim() : (req.socket?.remoteAddress || '0.0.0.0');
}

// ==========================================
// Routes
// ==========================================

// Step 1: Mod menu opens /?hwid=X&signature=S → show landing page
app.get('/', generateLimiter, verifySignature, async (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Step 2: User taps GENERATE KEY → create session → redirect to LootLabs
app.get('/generateKey', generateLimiter, verifySignature, async (req, res) => {
    const { hwid } = req.query;
    const now      = Date.now();
    const clientIp = getClientIp(req);

    try {
        await dbConnect();

        // Return existing valid key for this HWID
        const existingKey = await Key.findOne({ hwid, expiresAt: { $gt: now } });
        if (existingKey) {
            return res.redirect(`/?generated_key=${existingKey.keyString}&success=true`);
        }

        // Create session
        const sessionId = uuidv4();
        await Session.create({ sessionId, hwid, userIp: clientIp, createdAt: now });

        // Call LootLabs API to get encrypted ad link
        const checkpointUrl  = `${YOUR_DOMAIN}/checkpoint?session_id=${sessionId}`;
        const lootLabsApiUrl = `https://creators.lootlabs.gg/api/public/url_encryptor?destination_url=${encodeURIComponent(checkpointUrl)}&api_token=${LOOT_LINK_API_TOKEN}`;

        const lootRes = await axios.get(lootLabsApiUrl, { timeout: 10000 });

        if (lootRes.data && lootRes.data.message) {
            return res.redirect(`${LOOT_LINK_BASE_URL}&data=${lootRes.data.message}`);
        }

        console.error('LootLabs bad response:', JSON.stringify(lootRes.data));
        return res.status(500).send("Ad link generation failed. Please try again.");

    } catch (err) {
        console.error('generateKey error:', err.message || err);
        return res.status(500).send("Error generating session. Please try again later.");
    }
});

// Step 3: LootLabs redirects here after ad → create key
app.get('/checkpoint', async (req, res) => {
    const { session_id } = req.query;
    const referer = req.headers.referer || "";

    const allowedReferers = ["loot-link.com", "lootlabs.gg", "lootdest.org"];
    const isValidReferer = allowedReferers.some(r => referer.includes(r));
    if (!isValidReferer) {
        return res.status(403).send("Direct access is forbidden.");
    }

    try {
        await dbConnect();

        const session = await Session.findOne({ sessionId: session_id });
        if (!session) return res.status(400).send("Session expired. Please go back and try again.");

        const newKey   = "SACRED_" + crypto.randomBytes(4).toString('hex').toUpperCase();
        const expiresAt = Date.now() + (24 * 60 * 60 * 1000); // 24 hours

        // Upsert: one key per HWID (replaces old expired key)
        await Key.findOneAndUpdate(
            { hwid: session.hwid },
            { keyString: newKey, ipAddress: session.userIp, expiresAt },
            { upsert: true, new: true }
        );

        await Session.deleteOne({ sessionId: session_id });

        return res.redirect(`/?generated_key=${newKey}&success=true`);

    } catch (err) {
        console.error('checkpoint error:', err.message || err);
        return res.status(500).send("Verification failed. Please try again.");
    }
});

// Verify (called by C++ mod menu)
app.get('/verify', async (req, res) => {
    const { key, hwid } = req.query;
    if (!key || !hwid) return res.json({ success: false, reason: 'Missing params' });

    try {
        await dbConnect();

        const foundKey = await Key.findOne({ keyString: key, hwid });
        if (foundKey && Date.now() < foundKey.expiresAt) {
            return res.json({ success: true });
        }

        const reason = !foundKey
            ? 'Invalid key or wrong device'
            : 'Key expired';
        return res.json({ success: false, reason });

    } catch (err) {
        console.error('verify error:', err.message || err);
        return res.status(500).json({ success: false, reason: 'Server error' });
    }
});

// Admin panel
app.get('/admin', (req, res) => res.sendFile(path.join(__dirname, 'public', 'admin.html')));

// Admin API: list all keys
app.get('/admin/keys', async (req, res) => {
    const { password } = req.query;
    if (password !== ADMIN_PASSWORD) return res.status(403).json({ error: 'Unauthorized' });

    try {
        await dbConnect();
        const keys = await Key.find().sort({ expiresAt: -1 });
        const now  = Date.now();
        return res.json(keys.map(k => ({
            key:       k.keyString,
            hwid:      k.hwid,
            ip:        k.ipAddress,
            expires:   new Date(k.expiresAt).toISOString(),
            active:    k.expiresAt > now
        })));
    } catch (e) {
        return res.status(500).json({ error: 'DB error' });
    }
});

// Admin API: revoke key
app.delete('/admin/keys/:key', async (req, res) => {
    const { password } = req.query;
    if (password !== ADMIN_PASSWORD) return res.status(403).json({ error: 'Unauthorized' });

    try {
        await dbConnect();
        await Key.deleteOne({ keyString: req.params.key });
        return res.json({ success: true });
    } catch (e) {
        return res.status(500).json({ error: 'DB error' });
    }
});

// Static files (after route handlers so / isn't hijacked)
app.use(express.static(path.join(__dirname, 'public')));

// ==========================================
// Start (local dev only — Vercel ignores this)
// ==========================================
if (process.env.NODE_ENV !== 'production') {
    app.listen(port, () => console.log(`Server running on port ${port}`));
}

module.exports = app;
