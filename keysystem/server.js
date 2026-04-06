const express = require('express');
const mongoose = require('mongoose');
const axios = require('axios');
const { v4: uuidv4 } = require('uuid');
const crypto = require('crypto');
const path = require('path');
const cors = require('cors');
const rateLimit = require('express-rate-limit');

const app = express();
const port = 3000;

// https://ryukobs-generator.up.railway.app/?hwid=Nokia/DoctorDoom_00EEA/DRD_sprout:12/SKQ1.220119.001/00WW_3_46F:user/release-keys&signature=642d2006b1804089d44fe0b835b7d576
// ==========================================
// Configuration & Secrets
// ==========================================
const LOOT_LINK_API_TOKEN = "ba72281d3b361f5c42095b784e1559941d5730982a47c6701f122707afe57234"; 
const LOOT_LINK_BASE_URL = "https://lootdest.org/s?tEV6Qcgt"; 
const YOUR_DOMAIN = "https://ryukobs-generator.up.railway.app"; 
const SERVER_SECRET = "SJSIDIIDJEJRKRKRKKRDIIDIDKDKDKDKFKTJTJJFJFJJFFKFKKFKFKFKFIDIR";
const mongoURI = "mongodb+srv://ryukodevv:09090577409@cluster0.6ophm4c.mongodb.net/?appName=Cluster0"; 

// ==========================================
// Database Setup
// ==========================================
mongoose.connect(mongoURI)
    .then(() => console.log('Connected to MongoDB successfully!'))
    .catch(err => console.error('MongoDB connection error:', err));

const sessionSchema = new mongoose.Schema({
    sessionId: { type: String, required: true, unique: true },
    hwid: { type: String, required: true },
    userIp: { type: String }, // Track IP during session
    createdAt: { type: Number, required: true }
});
const Session = mongoose.model('Session', sessionSchema);

const keySchema = new mongoose.Schema({
    keyString: { type: String, required: true, unique: true },
    hwid: { type: String, required: true },
    ipAddress: { type: String, required: true }, // Saved IP
    expiresAt: { type: Number, required: true }
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

// Middleware to verify the C++ Mod Menu signature
const verifySignature = (req, res, next) => {
    const { hwid, signature } = req.query;
    if (!hwid || !signature) return res.status(403).send("Missing Security Parameters.");
    
    const expectedString = `hwid=${hwid}${SERVER_SECRET}`;
    const expectedHash = crypto.createHash('md5').update(expectedString).digest('hex');
    
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

app.use(express.static(path.join(__dirname, 'public')));

// ==========================================
// API Endpoints
// ==========================================
app.get('/', generateLimiter, verifySignature, blockVpn, async (req, res) => {
    
})
app.get('/generateKey', generateLimiter, verifySignature, blockVpn, async (req, res) => {
    const { hwid } = req.query;
    const now = Date.now();
    const forwarded = req.headers['x-forwarded-for'];
    const clientIp = forwarded ? forwarded.split(',')[0] : req.socket.remoteAddress;

    try {
        // --- IP & HWID Check: Return active key if exists ---
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

app.get('/checkpoint', async (req, res) => {
    const { session_id } = req.query;
    const referer = req.headers.referer || "";

    // Basic bypass protection
    if (!referer.includes("loot-link.com") && !referer.includes("lootlabs.gg") && !referer.includes("lootdest.org")) {
        return res.status(403).send("Direct access is forbidden.");
    }

    try {
        const session = await Session.findOne({ sessionId: session_id });
        if (!session) return res.status(400).send("Session expired.");

        const newKey = "RYUKO_" + crypto.randomBytes(4).toString('hex').toUpperCase();
        const expiresAt = Date.now() + (24 * 60 * 60 * 1000); 

        // Update key for this HWID and save the IP address
        await Key.findOneAndUpdate(
            { hwid: session.hwid },
            { 
                keyString: newKey, 
                ipAddress: session.userIp, 
                expiresAt: expiresAt 
            },
            { upsert: true }
        );

        await Session.deleteOne({ sessionId: session_id });
        res.redirect(`/?generated_key=${newKey}&success=true`);
    } catch (error) {
        res.status(500).send("Verification Failed.");
    }
});

app.get('/verify', async (req, res) => {
    const { key, hwid } = req.query;
    try {
        const foundKey = await Key.findOne({ keyString: key, hwid: hwid });
        if (foundKey && Date.now() < foundKey.expiresAt) {
            return res.json({ success: true });
        }
        res.json({ success: false });
    } catch (error) {
        res.status(500).json({ success: false });
    }
});

// Cleanup
setInterval(async () => {
    const now = Date.now();
    await Key.deleteMany({ expiresAt: { $lt: now } });
    await Session.deleteMany({ createdAt: { $lt: now - (30 * 60 * 1000) } });
}, 60 * 60 * 1000);

app.listen(port, () => console.log(`Server on port ${port}`));
