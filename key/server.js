const express = require('express');
const { neon } = require('@neondatabase/serverless');
const axios = require('axios');
const { v4: uuidv4 } = require('uuid');
const crypto = require('crypto');
const path = require('path');
const cors = require('cors');
const rateLimit = require('express-rate-limit');

const app = express();
const port = process.env.PORT || 5000;

app.use(express.json());

// ==========================================
// Configuration (hardcoded)
// ==========================================
const DATABASE_URL        = 'postgresql://neondb_owner:npg_hkG8lf3zrLKF@ep-cool-silence-anfi0aab-pooler.c-6.us-east-1.aws.neon.tech/neondb?sslmode=require&channel_binding=require';
const LOOT_LINK_API_TOKEN = '22044aa0aad01a7297c21a339ae4243b50cd62d12e67e0e0d712c1d6ab3fcd4f';
const LOOT_LINK_BASE_URL  = 'https://lootdest.org/s?muMuGbEY';
const YOUR_DOMAIN         = 'https://bskey.vercel.app';
const SERVER_SECRET       = 'SJSIDIIDJEJRKRKRKKRDIIDIDKDKDKDKFKTJTJJFJFJJFFKFKKFKFKFKFIDIR';
const ADMIN_PASSWORD      = 'joshua';

const sql = neon(DATABASE_URL);

// ==========================================
// Init DB
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
            hwid        VARCHAR(256),
            ip_address  VARCHAR(64),
            expires_at  BIGINT       NOT NULL,
            username    VARCHAR(64)  DEFAULT 'FREE_1DAY',
            revoked     BOOLEAN      DEFAULT FALSE,
            created_at  BIGINT
        )
    `;
    try {
        await sql`ALTER TABLE keys ADD CONSTRAINT keys_hwid_unique UNIQUE (hwid)`;
    } catch (_) {}
}
initDb().catch(err => console.error('initDb error:', err.message));

// ==========================================
// Helpers
// ==========================================
const generateLimiter = rateLimit({ windowMs: 15 * 60 * 1000, max: 10 });

app.use(cors());
app.set('trust proxy', 1);

function verifySignature(hwid, signature) {
    const expected = crypto.createHash('md5').update('hwid=' + hwid + SERVER_SECRET).digest('hex');
    return signature === expected;
}

function checkAdmin(req, res) {
    const pw = req.headers['x-admin-password'] || req.query.password;
    if (pw !== ADMIN_PASSWORD) {
        res.status(403).json({ error: 'Unauthorized' });
        return false;
    }
    return true;
}

function getClientIp(req) {
    const fwd = req.headers['x-forwarded-for'];
    return fwd ? fwd.split(',')[0].trim() : (req.socket?.remoteAddress || '0.0.0.0');
}

// ==========================================
// Routes
// ==========================================

// Landing page — no blanket signature check
// Handles: /?hwid=X&signature=S  AND  /?generated_key=X&success=true
app.get('/', generateLimiter, (req, res) => {
    const { hwid, signature, success, generated_key } = req.query;

    // Success redirect after ad — serve page directly
    if (success === 'true' && generated_key) {
        return res.sendFile(path.join(__dirname, 'public', 'index.html'));
    }

    // Mod menu access — require valid signature
    if (!hwid || !signature) return res.status(403).send('Missing Security Parameters.');
    if (!verifySignature(hwid, signature)) return res.status(403).send('Bypass Attempt Detected.');

    return res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Step 2: User taps GENERATE KEY → create session → LootLabs ad
app.get('/generateKey', generateLimiter, async (req, res) => {
    const { hwid, signature } = req.query;
    if (!hwid || !signature) return res.status(403).send('Missing Security Parameters.');
    if (!verifySignature(hwid, signature)) return res.status(403).send('Bypass Attempt Detected.');

    const now      = Date.now();
    const clientIp = getClientIp(req);

    try {
        const existing = await sql`
            SELECT key_string FROM keys
            WHERE hwid = ${hwid}
              AND expires_at > ${now}
              AND (revoked = false OR revoked IS NULL)
            LIMIT 1
        `;
        if (existing.length > 0) {
            return res.redirect(`/?generated_key=${existing[0].key_string}&success=true`);
        }

        const sessionId = uuidv4();
        await sql`
            INSERT INTO sessions (session_id, hwid, user_ip, created_at)
            VALUES (${sessionId}, ${hwid}, ${clientIp}, ${now})
            ON CONFLICT (session_id) DO NOTHING
        `;

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

// Step 3: After ad → generate key
app.get('/checkpoint', async (req, res) => {
    const { session_id } = req.query;
    const referer = req.headers.referer || '';
    const validReferers = ['loot-link.com', 'lootlabs.gg', 'lootdest.org'];
    if (!validReferers.some(r => referer.includes(r))) {
        return res.status(403).send('Direct access is forbidden.');
    }

    try {
        const rows = await sql`SELECT * FROM sessions WHERE session_id = ${session_id} LIMIT 1`;
        if (rows.length === 0) return res.status(400).send('Session expired. Please go back and try again.');

        const session   = rows[0];
        const newKey    = 'SACRED_' + crypto.randomBytes(4).toString('hex').toUpperCase();
        const expiresAt = Date.now() + (24 * 60 * 60 * 1000);

        await sql`DELETE FROM keys WHERE hwid = ${session.hwid}`;
        await sql`
            INSERT INTO keys (key_string, hwid, ip_address, expires_at, username, revoked, created_at)
            VALUES (${newKey}, ${session.hwid}, ${session.user_ip}, ${expiresAt}, 'FREE_1DAY', false, ${Date.now()})
        `;

        await sql`DELETE FROM sessions WHERE session_id = ${session_id}`;
        return res.redirect(`/?generated_key=${newKey}&success=true`);

    } catch (err) {
        console.error('checkpoint error:', err.message || err);
        return res.status(500).send('Verification failed. Please try again.');
    }
});

// Verify — handles both FREE keys (hwid already set) and VIP keys (hwid null = first activation)
app.get('/verify', async (req, res) => {
    const { key, hwid } = req.query;
    if (!key || !hwid) return res.json({ success: false, reason: 'Missing params' });

    try {
        const rows = await sql`SELECT * FROM keys WHERE key_string = ${key} LIMIT 1`;

        if (rows.length === 0) {
            return res.json({ success: false, reason: 'Invalid key' });
        }

        const found = rows[0];

        if (found.revoked) {
            return res.json({ success: false, reason: 'Key revoked by admin' });
        }

        if (Date.now() >= Number(found.expires_at)) {
            return res.json({ success: false, reason: 'Key expired' });
        }

        // VIP key — hwid is null (not yet activated) → bind this device
        if (!found.hwid) {
            await sql`UPDATE keys SET hwid = ${hwid} WHERE key_string = ${key}`;
            return res.json({ success: true });
        }

        // Key already bound — check device matches
        if (found.hwid !== hwid) {
            return res.json({ success: false, reason: 'Key bound to different device' });
        }

        return res.json({ success: true });

    } catch (err) {
        console.error('verify error:', err.message || err);
        return res.status(500).json({ success: false, reason: 'Server error' });
    }
});

// ==========================================
// Admin Routes
// ==========================================

app.get('/admin', (req, res) => res.sendFile(path.join(__dirname, 'public', 'admin.html')));

// Admin login
app.post('/admin/login', (req, res) => {
    const { password } = req.body;
    if (password !== ADMIN_PASSWORD) return res.json({ success: false });
    res.json({ success: true });
});

// Admin stats
app.get('/admin/stats', async (req, res) => {
    if (!checkAdmin(req, res)) return;
    try {
        const now   = Date.now();
        const all   = await sql`SELECT revoked, expires_at FROM keys`;
        const total  = all.length;
        const active = all.filter(k => !k.revoked && Number(k.expires_at) > now).length;
        const expired = all.filter(k => Number(k.expires_at) <= now).length;
        const revoked = all.filter(k => k.revoked).length;
        res.json({ total, active, expired, revoked });
    } catch (e) {
        res.status(500).json({ error: 'DB error' });
    }
});

// List all keys
app.get('/admin/keys', async (req, res) => {
    if (!checkAdmin(req, res)) return;
    try {
        const keys = await sql`SELECT * FROM keys ORDER BY created_at DESC`;
        const now  = Date.now();
        res.json(keys.map(k => ({
            key:      k.key_string,
            hwid:     k.hwid || null,
            ip:       k.ip_address,
            username: k.username,
            expires:  new Date(Number(k.expires_at)).toISOString(),
            active:   Number(k.expires_at) > now && !k.revoked,
            revoked:  k.revoked
        })));
    } catch (e) {
        res.status(500).json({ error: 'DB error' });
    }
});

// Generate VIP key (admin creates key without hwid — user activates on first login)
app.post('/admin/generate', async (req, res) => {
    if (!checkAdmin(req, res)) return;
    try {
        const { days = 30, username: labelRaw, count = 1 } = req.body;
        const label     = labelRaw || `VIP_${days}D`;
        const expiresAt = Date.now() + (Number(days) * 24 * 60 * 60 * 1000);
        const generatedKeys = [];

        for (let i = 0; i < Math.min(Number(count), 50); i++) {
            const keyString = 'SACRED_' + crypto.randomBytes(6).toString('hex').toUpperCase();
            await sql`
                INSERT INTO keys (key_string, hwid, ip_address, expires_at, username, revoked, created_at)
                VALUES (${keyString}, NULL, NULL, ${expiresAt}, ${label}, false, ${Date.now()})
            `;
            generatedKeys.push(keyString);
        }
        // Return single-key format (for UI) and full list
        res.json({ success: true, key: generatedKeys[0], expiresAt, keys: generatedKeys });
    } catch (e) {
        console.error('generate error:', e.message);
        res.status(500).json({ error: 'DB error' });
    }
});

// Revoke key
app.post('/admin/revoke', async (req, res) => {
    if (!checkAdmin(req, res)) return;
    try {
        const { key } = req.body;
        await sql`UPDATE keys SET revoked = true WHERE key_string = ${key}`;
        res.json({ success: true });
    } catch (e) {
        res.status(500).json({ error: 'DB error' });
    }
});

// Delete key
app.post('/admin/delete', async (req, res) => {
    if (!checkAdmin(req, res)) return;
    try {
        const { key } = req.body;
        await sql`DELETE FROM keys WHERE key_string = ${key}`;
        res.json({ success: true });
    } catch (e) {
        res.status(500).json({ error: 'DB error' });
    }
});

// Static files (after route handlers)
app.use(express.static(path.join(__dirname, 'public')));

// ==========================================
// Start (local only)
// ==========================================
if (process.env.NODE_ENV !== 'production') {
    app.listen(port, () => console.log(`Server running on port ${port}`));
}

module.exports = app;
