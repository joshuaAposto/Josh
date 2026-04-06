const express = require('express');
const { neon } = require('@neondatabase/serverless');
const axios = require('axios');
const { v4: uuidv4 } = require('uuid');
const crypto = require('crypto');
const path = require('path');
const cors = require('cors');
const rateLimit = require('express-rate-limit');

const app = express();
const port = process.env.PORT || 3000;

// ==========================================
// Configuration (hardcoded)
// ==========================================
const DATABASE_URL      = 'postgresql://neondb_owner:npg_hkG8lf3zrLKF@ep-cool-silence-anfi0aab-pooler.c-6.us-east-1.aws.neon.tech/neondb?sslmode=require&channel_binding=require';
const LOOT_LINK_API_TOKEN = 'ba72281d3b361f5c42095b784e1559941d5730982a47c6701f122707afe57234';
const LOOT_LINK_BASE_URL  = 'https://lootdest.org/s?tEV6Qcgt';
const YOUR_DOMAIN         = 'https://bskey.vercel.app';
const SERVER_SECRET       = 'SJSIDIIDJEJRKRKRKKRDIIDIDKDKDKDKFKTJTJJFJFJJFFKFKKFKFKFKFIDIR';
const ADMIN_PASSWORD      = 'joshua';

const sql = neon(DATABASE_URL);

// ==========================================
// Init DB tables
// ==========================================
async function initDb() {
    await sql`
        CREATE TABLE IF NOT EXISTS sessions (
            session_id  VARCHAR(64)  PRIMARY KEY,
            hwid        VARCHAR(256) NOT NULL,
            user_ip     VARCHAR(64),
            created_at  BIGINT       NOT NULL
        )
    `;
    await sql`
        CREATE TABLE IF NOT EXISTS keys (
            id          SERIAL       PRIMARY KEY,
            key_string  VARCHAR(64)  UNIQUE NOT NULL,
            hwid        VARCHAR(256) UNIQUE NOT NULL,
            ip_address  VARCHAR(64),
            expires_at  BIGINT       NOT NULL
        )
    `;
}
initDb().catch(err => console.error('initDb error:', err.message));

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
    if (!hwid || !signature) return res.status(403).send('Missing Security Parameters.');
    const expected = crypto.createHash('md5').update('hwid=' + hwid + SERVER_SECRET).digest('hex');
    if (signature !== expected) return res.status(403).send('Bypass Attempt Detected.');
    next();
};

function getClientIp(req) {
    const fwd = req.headers['x-forwarded-for'];
    return fwd ? fwd.split(',')[0].trim() : (req.socket?.remoteAddress || '0.0.0.0');
}

// ==========================================
// Routes
// ==========================================

// Step 1: Mod menu opens /?hwid=X&signature=S → show landing page
app.get('/', generateLimiter, verifySignature, (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Step 2: User taps GENERATE KEY → create session → LootLabs ad
app.get('/generateKey', generateLimiter, verifySignature, async (req, res) => {
    const { hwid } = req.query;
    const now      = Date.now();
    const clientIp = getClientIp(req);

    try {
        // Return existing valid key for this HWID
        const existing = await sql`
            SELECT key_string FROM keys
            WHERE hwid = ${hwid} AND expires_at > ${now}
            LIMIT 1
        `;
        if (existing.length > 0) {
            return res.redirect(`/?generated_key=${existing[0].key_string}&success=true`);
        }

        // Create session
        const sessionId = uuidv4();
        await sql`
            INSERT INTO sessions (session_id, hwid, user_ip, created_at)
            VALUES (${sessionId}, ${hwid}, ${clientIp}, ${now})
        `;

        // Call LootLabs API
        const checkpointUrl  = `${YOUR_DOMAIN}/checkpoint?session_id=${sessionId}`;
        const lootLabsApiUrl = `https://creators.lootlabs.gg/api/public/url_encryptor?destination_url=${encodeURIComponent(checkpointUrl)}&api_token=${LOOT_LINK_API_TOKEN}`;

        const lootRes = await axios.get(lootLabsApiUrl, { timeout: 10000 });

        if (lootRes.data && lootRes.data.message) {
            return res.redirect(`${LOOT_LINK_BASE_URL}&data=${lootRes.data.message}`);
        }

        console.error('LootLabs bad response:', JSON.stringify(lootRes.data));
        return res.status(500).send('Ad link generation failed. Please try again.');

    } catch (err) {
        console.error('generateKey error:', err.message || err);
        return res.status(500).send('Error generating session. Please try again later.');
    }
});

// Step 3: LootLabs redirects here after ad → create key
app.get('/checkpoint', async (req, res) => {
    const { session_id } = req.query;
    const referer = req.headers.referer || '';

    const validReferers = ['loot-link.com', 'lootlabs.gg', 'lootdest.org'];
    if (!validReferers.some(r => referer.includes(r))) {
        return res.status(403).send('Direct access is forbidden.');
    }

    try {
        const rows = await sql`
            SELECT * FROM sessions WHERE session_id = ${session_id} LIMIT 1
        `;
        if (rows.length === 0) return res.status(400).send('Session expired. Please go back and try again.');

        const session   = rows[0];
        const newKey    = 'SACRED_' + crypto.randomBytes(4).toString('hex').toUpperCase();
        const expiresAt = Date.now() + (24 * 60 * 60 * 1000); // 24 hours

        // Upsert: one key per HWID
        await sql`
            INSERT INTO keys (key_string, hwid, ip_address, expires_at)
            VALUES (${newKey}, ${session.hwid}, ${session.user_ip}, ${expiresAt})
            ON CONFLICT (hwid) DO UPDATE
                SET key_string = EXCLUDED.key_string,
                    ip_address = EXCLUDED.ip_address,
                    expires_at = EXCLUDED.expires_at
        `;

        await sql`DELETE FROM sessions WHERE session_id = ${session_id}`;

        return res.redirect(`/?generated_key=${newKey}&success=true`);

    } catch (err) {
        console.error('checkpoint error:', err.message || err);
        return res.status(500).send('Verification failed. Please try again.');
    }
});

// Verify (called by C++ mod menu)
app.get('/verify', async (req, res) => {
    const { key, hwid } = req.query;
    if (!key || !hwid) return res.json({ success: false, reason: 'Missing params' });

    try {
        const rows = await sql`
            SELECT * FROM keys WHERE key_string = ${key} AND hwid = ${hwid} LIMIT 1
        `;
        if (rows.length > 0 && Date.now() < rows[0].expires_at) {
            return res.json({ success: true });
        }
        const reason = rows.length === 0 ? 'Invalid key or wrong device' : 'Key expired';
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
        const keys = await sql`SELECT * FROM keys ORDER BY expires_at DESC`;
        const now  = Date.now();
        return res.json(keys.map(k => ({
            key:     k.key_string,
            hwid:    k.hwid,
            ip:      k.ip_address,
            expires: new Date(Number(k.expires_at)).toISOString(),
            active:  Number(k.expires_at) > now
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
        await sql`DELETE FROM keys WHERE key_string = ${req.params.key}`;
        return res.json({ success: true });
    } catch (e) {
        return res.status(500).json({ error: 'DB error' });
    }
});

// Static files (after route handlers so / isn't hijacked when no params)
app.use(express.static(path.join(__dirname, 'public')));

// ==========================================
// Start (local only — Vercel uses module.exports)
// ==========================================
if (process.env.NODE_ENV !== 'production') {
    app.listen(port, () => console.log(`Server running on port ${port}`));
}

module.exports = app;
