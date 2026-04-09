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
const DATABASE_URL        = 'postgresql://neondb_owner:npg_hkG8lf3zrLKF@ep-cool-silence-anfi0aab-pooler.c-6.us-east-1.aws.neon.tech/neondb?sslmode=require';
const LOOT_LINK_API_TOKEN = '22044aa0aad01a7297c21a339ae4243b50cd62d12e67e0e0d712c1d6ab3fcd4f';
const LOOT_LINK_BASE_URL  = 'https://lootdest.org/s?muMuGbEY';
const YOUR_DOMAIN         = 'https://bskey.replit.app';
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
    // Ensure no hwid unique constraint exists (allows VIP + free keys for same device)
    try {
        await sql`ALTER TABLE keys DROP CONSTRAINT IF EXISTS keys_hwid_unique`;
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
app.get('/', (req, res) => {
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

// Lightweight check on page load — does NOT create a session or call LootLabs
// Called by autoCheckKey() so the rate limiter is NOT consumed until user taps the button
app.get('/checkKey', async (req, res) => {
    const { hwid, signature } = req.query;
    if (!hwid || !signature) return res.json({ valid: false, error: 'Missing params' });
    if (!verifySignature(hwid, signature)) return res.json({ valid: false, error: 'Invalid signature' });
    try {
        const now = Date.now();
        const rows = await sql`
            SELECT key_string, expires_at FROM keys
            WHERE hwid = ${hwid}
              AND username = 'FREE_1DAY'
              AND expires_at > ${now}
              AND (revoked = false OR revoked IS NULL)
            LIMIT 1
        `;
        if (rows.length > 0) return res.json({ valid: true, key: rows[0].key_string });
        return res.json({ valid: false });
    } catch (err) {
        console.error('checkKey error:', err.message || err);
        return res.json({ valid: false, error: 'Server error' });
    }
});

// Step 2: User taps GENERATE KEY → create session → return LootLabs ad URL as JSON
app.get('/generateKey', generateLimiter, async (req, res) => {
    const { hwid, signature } = req.query;
    if (!hwid || !signature) return res.json({ error: 'Missing Security Parameters.' });
    if (!verifySignature(hwid, signature)) return res.json({ error: 'Bypass Attempt Detected.' });

    const now      = Date.now();
    const clientIp = getClientIp(req);

    try {
        // Return existing valid FREE key directly (no redirect needed)
        const existing = await sql`
            SELECT key_string FROM keys
            WHERE hwid = ${hwid}
              AND username = 'FREE_1DAY'
              AND expires_at > ${now}
              AND (revoked = false OR revoked IS NULL)
            LIMIT 1
        `;
        if (existing.length > 0) {
            return res.json({ key: existing[0].key_string });
        }

        // Create session
        const sessionId = uuidv4();
        await sql`
            INSERT INTO sessions (session_id, hwid, user_ip, created_at)
            VALUES (${sessionId}, ${hwid}, ${clientIp}, ${now})
            ON CONFLICT (session_id) DO NOTHING
        `;

        // Get LootLabs encrypted link
        const checkpointUrl  = `${YOUR_DOMAIN}/checkpoint?session_id=${sessionId}`;
        const lootLabsApiUrl = `https://creators.lootlabs.gg/api/public/url_encryptor?destination_url=${encodeURIComponent(checkpointUrl)}&api_token=${LOOT_LINK_API_TOKEN}`;

        const lootRes = await axios.get(lootLabsApiUrl, { timeout: 7000 });
        if (lootRes.data && lootRes.data.message) {
            return res.json({ redirect: `${LOOT_LINK_BASE_URL}&data=${lootRes.data.message}` });
        }

        console.error('LootLabs bad response:', JSON.stringify(lootRes.data));
        return res.json({ error: 'Ad link generation failed. Please try again.' });

    } catch (err) {
        console.error('generateKey error:', err.message || err);
        return res.json({ error: 'Server timeout. Please try again.' });
    }
});

// Step 3: After ad → generate key
app.get('/checkpoint', async (req, res) => {
    const { session_id } = req.query;
    if (!session_id) return res.status(400).send('Missing session. Please go back and try again.');

    try {
        const rows = await sql`SELECT * FROM sessions WHERE session_id = ${session_id} LIMIT 1`;
        if (rows.length === 0) return res.status(400).send('Session expired. Please go back and try again.');

        const session   = rows[0];
        const newKey    = 'SACRED_' + crypto.randomBytes(4).toString('hex').toUpperCase();
        const expiresAt = Date.now() + (24 * 60 * 60 * 1000);

        await sql`DELETE FROM keys WHERE hwid = ${session.hwid} AND username = 'FREE_1DAY'`;
        await sql`
            INSERT INTO keys (key_string, hwid, ip_address, expires_at, username, revoked, created_at)
            VALUES (${newKey}, ${session.hwid}, ${session.user_ip}, ${expiresAt}, 'FREE_1DAY', false, ${Date.now()})
        `;

        await sql`DELETE FROM sessions WHERE session_id = ${session_id}`;
        await sql`DELETE FROM sessions WHERE created_at < ${Date.now() - (60 * 60 * 1000)}`;
        return res.redirect(`/?generated_key=${newKey}&success=true`);

    } catch (err) {
        console.error('checkpoint error:', err.message || err);
        return res.status(500).send('Verification failed. Please try again.');
    }
});

// Helper: build a signed verify response (MD5 seal + timestamp for anti-crack / anti-replay)
function buildVerifyResponse(isSuccess, message, hwid) {
    const timestamp  = Date.now().toString();
    const dataToHash = SERVER_SECRET + isSuccess.toString() + timestamp + hwid;
    const sig        = crypto.createHash('md5').update(dataToHash).digest('hex');
    return { success: isSuccess, message, ts: timestamp, sig };
}

// Verify — handles both FREE keys (hwid already set) and VIP keys (hwid null = first activation)
app.get('/verify', async (req, res) => {
    const { key, hwid } = req.query;
    if (!key || !hwid) {
        return res.json(buildVerifyResponse(false, 'Missing params', hwid || ''));
    }

    try {
        const rows = await sql`SELECT * FROM keys WHERE key_string = ${key} LIMIT 1`;

        if (rows.length === 0) {
            return res.json(buildVerifyResponse(false, 'Invalid key', hwid));
        }

        const found = rows[0];

        if (found.revoked) {
            return res.json(buildVerifyResponse(false, 'Key revoked by admin', hwid));
        }

        if (Date.now() >= Number(found.expires_at)) {
            return res.json(buildVerifyResponse(false, 'Key expired', hwid));
        }

        // Compute days remaining
        const msLeft        = Math.max(0, Number(found.expires_at) - Date.now());
        const daysRemaining = Math.ceil(msLeft / 86400000);
        const username      = found.username || 'USER';
        const isFreeKey     = found.username === 'FREE_1DAY';

        if (isFreeKey) {
            // FREE key: device-locked — hwid must match exactly; NULL hwid means admin reset it (must regenerate)
            if (!found.hwid || found.hwid !== hwid) {
                const reason = !found.hwid
                    ? 'Free key was reset. Generate a new one at bskey.replit.app'
                    : 'This free key was generated for a different device. Get your own at bskey.replit.app';
                return res.json(buildVerifyResponse(false, reason, hwid));
            }
            const payload = buildVerifyResponse(true, `Welcome back, ${username}! ${daysRemaining}d left`, hwid);
            return res.json({ ...payload, days_remaining: daysRemaining, username });
        }

        // VIP key: NO device binding — any phone/device can use it freely
        const payload = buildVerifyResponse(true, `Welcome back, ${username}! ${daysRemaining}d left`, hwid);
        return res.json({ ...payload, days_remaining: daysRemaining, username });

    } catch (err) {
        console.error('verify error:', err.message || err);
        return res.json(buildVerifyResponse(false, 'Server error', hwid));
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
        res.json(keys.map(k => {
            const expiresAt     = Number(k.expires_at);
            const createdAt     = Number(k.created_at) || (expiresAt - 86400000);
            const expired       = expiresAt <= now;
            const totalMs       = expiresAt - createdAt;
            const days          = Math.max(1, Math.round(totalMs / 86400000));
            const msLeft        = Math.max(0, expiresAt - now);
            const daysRemaining = Math.ceil(msLeft / 86400000);
            const activated     = !!k.hwid;
            return {
                key:          k.key_string,
                hwid:         k.hwid || null,
                ip:           k.ip_address,
                username:     k.username,
                expiresAt:    expiresAt,
                days:         days,
                daysRemaining: daysRemaining,
                expired:      expired,
                activated:    activated,
                active:       !expired && !k.revoked,
                revoked:      k.revoked
            };
        }));
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

// Reset HWID binding
// - FREE_1DAY keys: deleted entirely so user must go through the ad flow to get a new key
// - VIP keys: hwid set to NULL so a new device can activate the key
app.post('/admin/reset-hwid', async (req, res) => {
    if (!checkAdmin(req, res)) return;
    try {
        const { key } = req.body;
        if (!key) return res.status(400).json({ error: 'key required' });

        const rows = await sql`SELECT key_string, username FROM keys WHERE key_string = ${key} AND revoked = FALSE LIMIT 1`;
        if (!rows.length) return res.status(404).json({ error: 'Key not found or revoked' });

        const isFreeKey = rows[0].username === 'FREE_1DAY';

        if (isFreeKey) {
            // Delete free key — user must regenerate via the ad flow
            await sql`DELETE FROM keys WHERE key_string = ${key}`;
            return res.json({ success: true, message: 'Free key deleted — user must regenerate a new one via the ad flow' });
        } else {
            // Unbind VIP key so a new device can activate it
            await sql`UPDATE keys SET hwid = NULL WHERE key_string = ${key}`;
            return res.json({ success: true, message: 'HWID unbound — key can now be activated on a new device' });
        }
    } catch (e) {
        res.status(500).json({ error: 'DB error' });
    }
});

// Renew key — extends expiry by N days; works on expired AND active keys; clears revoked flag
app.post('/admin/renew', async (req, res) => {
    if (!checkAdmin(req, res)) return;
    try {
        const { key, days } = req.body;
        if (!key || !days || days < 1 || days > 3650) return res.status(400).json({ error: 'key and days (1-3650) required' });

        const rows = await sql`SELECT expires_at FROM keys WHERE key_string = ${key} LIMIT 1`;
        if (!rows.length) return res.status(404).json({ error: 'Key not found' });

        const now        = Date.now();
        const currentExp = Number(rows[0].expires_at);
        // If still active, extend from current expiry; if expired, restart from now
        const base       = currentExp > now ? currentExp : now;
        const newExpiry  = base + days * 86400000;

        await sql`
            UPDATE keys
            SET expires_at = ${newExpiry},
                revoked    = FALSE
            WHERE key_string = ${key}
        `;

        const msLeft        = newExpiry - now;
        const daysRemaining = Math.ceil(msLeft / 86400000);
        res.json({ success: true, newExpiresAt: newExpiry, daysRemaining });
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
