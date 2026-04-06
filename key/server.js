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
// Database Setup
// ==========================================
mongoose.connect(mongoURI)
    .then(() => console.log('Connected to MongoDB successfully!'))
    .catch(err => console.error('MongoDB connection error:', err));

const sessionSchema = new mongoose.Schema({
    sessionId: { type: String, required: true, unique: true },
    hwid:      { type: String, required: true },
    userIp:    { type: String },
    createdAt: { type: Number, required: true }
});
const Session = mongoose.model('Session', sessionSchema);

const keySchema = new mongoose.Schema({
    keyString:  { type: String, required: true, unique: true },
    hwid:       { type: String, required: true },
    ipAddress:  { type: String, required: true },
    expiresAt:  { type: Number, required: true }
});
const Key = mongoose.model('Key', keySchema);

// ==========================================
// Security Middlewares
// ==========================================
app.use(cors());

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

const blockVpn = async (req, res, next) => {
    const forwarded = req.headers['x-forwarded-for'];
    const clientIp = forwarded ? forwarded.split(',')[0] : req.socket.remoteAddress;
    if (clientIp === '::1' || clientIp === '127.0.0.1') return next();
    try {
        const response = await axios.get(`http://ip-api.com/json/${clientIp}?fields=proxy,hosting`);
        if (response.data.proxy || response.data.hosting) {
            return res.status(403).send("VPN/Proxy is not allowed.");
        }
        next();
    } catch (error) { next(); }
};

// ==========================================
// API Endpoints
// ==========================================

// Step 1: Mod menu opens this URL → verifies signature → redirects to LootLabs
app.get('/', generateLimiter, verifySignature, blockVpn, async (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Step 2: User taps GENERATE KEY on landing page → creates session → LootLabs ad
app.get('/generateKey', generateLimiter, verifySignature, blockVpn, async (req, res) => {
    const { hwid } = req.query;
    const now = Date.now();
    const forwarded = req.headers['x-forwarded-for'];
    const clientIp = forwarded ? forwarded.split(',')[0] : req.socket.remoteAddress;

    try {
        // Return existing active key if HWID or IP already has one
        const existingKey = await Key.findOne({
            $or: [{ hwid: hwid }, { ipAddress: clientIp }],
            expiresAt: { $gt: now }
        });

        if (existingKey) {
            return res.redirect(`/?generated_key=${existingKey.keyString}&success=true`);
        }

        const sessionId = uuidv4();
        await Session.create({ sessionId, hwid, userIp: clientIp, createdAt: now });

        const checkpointUrl = `${YOUR_DOMAIN}/checkpoint?session_id=${sessionId}`;
        const lootLabsApiUrl = `https://creators.lootlabs.gg/api/public/url_encryptor?destination_url=${encodeURIComponent(checkpointUrl)}&api_token=${LOOT_LINK_API_TOKEN}`;

        const response = await axios.get(lootLabsApiUrl);

        if (response.data && response.data.message) {
            res.redirect(`${LOOT_LINK_BASE_URL}&data=${response.data.message}`);
        } else {
            throw new Error("LootLabs API error");
        }
    } catch (error) {
        res.status(500).send("Error generating session.");
    }
});

// Step 3: After ad → LootLabs sends user here → generate key
app.get('/checkpoint', async (req, res) => {
    const { session_id } = req.query;
    const referer = req.headers.referer || "";

    if (!referer.includes("loot-link.com") && !referer.includes("lootlabs.gg") && !referer.includes("lootdest.org")) {
        return res.status(403).send("Direct access is forbidden.");
    }

    try {
        const session = await Session.findOne({ sessionId: session_id });
        if (!session) return res.status(400).send("Session expired.");

        const newKey = "SACRED_" + crypto.randomBytes(4).toString('hex').toUpperCase();
        const expiresAt = Date.now() + (24 * 60 * 60 * 1000);

        await Key.findOneAndUpdate(
            { hwid: session.hwid },
            { keyString: newKey, ipAddress: session.userIp, expiresAt: expiresAt },
            { upsert: true }
        );

        await Session.deleteOne({ sessionId: session_id });
        res.redirect(`/?generated_key=${newKey}&success=true`);
    } catch (error) {
        res.status(500).send("Verification Failed.");
    }
});

// Verify key (called by C++ mod menu)
app.get('/verify', async (req, res) => {
    const { key, hwid } = req.query;
    if (!key || !hwid) return res.json({ success: false, reason: 'Missing params' });
    try {
        const foundKey = await Key.findOne({ keyString: key, hwid: hwid });
        if (foundKey && Date.now() < foundKey.expiresAt) {
            return res.json({ success: true });
        }
        res.json({ success: false, reason: foundKey ? 'Key expired' : 'Invalid key or wrong device' });
    } catch (error) {
        res.status(500).json({ success: false, reason: 'Server error' });
    }
});

// Admin panel
app.get('/admin', (req, res) => res.sendFile(path.join(__dirname, 'public', 'admin.html')));

// Admin API: list all keys
app.get('/admin/keys', async (req, res) => {
    const { password } = req.query;
    if (password !== ADMIN_PASSWORD) return res.status(403).json({ error: 'Unauthorized' });
    try {
        const keys = await Key.find().sort({ expiresAt: -1 });
        const now = Date.now();
        const result = keys.map(k => ({
            key:       k.keyString,
            hwid:      k.hwid || 'Not activated',
            ip:        k.ipAddress,
            expires:   new Date(k.expiresAt).toISOString(),
            active:    k.expiresAt > now,
            activated: !!k.hwid
        }));
        res.json(result);
    } catch (e) {
        res.status(500).json({ error: 'DB error' });
    }
});

// Admin API: revoke/delete key
app.delete('/admin/keys/:key', async (req, res) => {
    const { password } = req.query;
    if (password !== ADMIN_PASSWORD) return res.status(403).json({ error: 'Unauthorized' });
    try {
        await Key.deleteOne({ keyString: req.params.key });
        res.json({ success: true });
    } catch (e) {
        res.status(500).json({ error: 'DB error' });
    }
});

// ==========================================
// Static files (must be after route handlers)
// ==========================================
app.use(express.static(path.join(__dirname, 'public')));

// ==========================================
// Cleanup expired keys & sessions every hour
// ==========================================
setInterval(async () => {
    const now = Date.now();
    await Key.deleteMany({ expiresAt: { $lt: now } });
    await Session.deleteMany({ createdAt: { $lt: now - (30 * 60 * 1000) } });
}, 60 * 60 * 1000);

// ==========================================
// Start server
// ==========================================
app.listen(port, () => console.log(`Server running on port ${port}`));

module.exports = app;
