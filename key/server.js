const express = require('express');
const { neon } = require('@neondatabase/serverless');
const crypto = require('crypto');
const path = require('path');
const cors = require('cors');
const rateLimit = require('express-rate-limit');

const app = express();
app.use(express.json());
app.use(cors());
// NOTE: express.static is registered AFTER route handlers below,
// so GET / with ?hwid params is intercepted first.

// ==========================================
// Configuration (hardcoded)
// ==========================================
const DATABASE_URL  = 'postgresql://neondb_owner:npg_hkG8lf3zrLKF@ep-cool-silence-anfi0aab-pooler.c-6.us-east-1.aws.neon.tech/neondb?sslmode=require&channel_binding=require';
const ADMIN_PASSWORD = 'joshua';
const SERVER_SECRET  = 'SJSIDIIDJEJRKRKRKKRDIIDIDKDKDKDKFKTJTJJFJFJJFFKFKKFKFKFKFIDIR';

const sql = neon(DATABASE_URL);

// ==========================================
// Init: create table if not exists
// ==========================================
async function initDb() {
    await sql`
        CREATE TABLE IF NOT EXISTS keys (
            id          SERIAL PRIMARY KEY,
            key_string  VARCHAR(64)  UNIQUE NOT NULL,
            username    VARCHAR(128) NOT NULL,
            hwid        VARCHAR(256) DEFAULT NULL,
            ip_address  VARCHAR(64)  DEFAULT NULL,
            created_at  BIGINT NOT NULL,
            expires_at  BIGINT NOT NULL,
            days        INTEGER NOT NULL,
            revoked     BOOLEAN DEFAULT FALSE
        )
    `;
    console.log('DB ready');
}
initDb().catch(console.error);

// ==========================================
// Middleware
// ==========================================
const verifyAdmin = (req, res, next) => {
    const pw = req.headers['x-admin-password'] || req.body?.adminPassword;
    if (pw !== ADMIN_PASSWORD) return res.status(401).json({ error: 'Unauthorized' });
    next();
};

const apiLimiter    = rateLimit({ windowMs: 60 * 1000, max: 30 });
const verifyLimiter = rateLimit({ windowMs: 60 * 1000, max: 60 });

// ==========================================
// ADMIN API
// ==========================================

// Verify admin password
app.post('/admin/login', (req, res) => {
    const { password } = req.body;
    if (password === ADMIN_PASSWORD) return res.json({ success: true });
    res.status(401).json({ success: false, error: 'Wrong password' });
});

// Generate a new key (admin only)
app.post('/admin/generate', apiLimiter, verifyAdmin, async (req, res) => {
    const { username, days } = req.body;
    if (!username || !days || days < 1 || days > 365) {
        return res.status(400).json({ error: 'Invalid username or days (1-365)' });
    }

    const keyString = 'SACRED_' + crypto.randomBytes(6).toString('hex').toUpperCase();
    const now       = Date.now();
    const expiresAt = now + (Number(days) * 24 * 60 * 60 * 1000);

    try {
        await sql`
            INSERT INTO keys (key_string, username, created_at, expires_at, days, revoked)
            VALUES (${keyString}, ${username}, ${now}, ${expiresAt}, ${Number(days)}, false)
        `;
        res.json({ success: true, key: keyString, username, days: Number(days), expiresAt });
    } catch (e) {
        res.status(500).json({ error: 'Failed to generate key', detail: e.message });
    }
});

// List all keys (admin only)
app.get('/admin/keys', verifyAdmin, async (req, res) => {
    try {
        const rows = await sql`SELECT * FROM keys ORDER BY created_at DESC LIMIT 200`;
        const now = Date.now();
        res.json(rows.map(k => ({
            key:          k.key_string,
            username:     k.username,
            hwid:         k.hwid || 'Not activated',
            days:         k.days,
            createdAt:    Number(k.created_at),
            expiresAt:    Number(k.expires_at),
            daysRemaining:Math.max(0, Math.ceil((Number(k.expires_at) - now) / 86400000)),
            revoked:      k.revoked,
            activated:    !!k.hwid,
            expired:      now > Number(k.expires_at)
        })));
    } catch (e) {
        res.status(500).json({ error: e.message });
    }
});

// Revoke a key (admin only)
app.post('/admin/revoke', verifyAdmin, async (req, res) => {
    const { key } = req.body;
    if (!key) return res.status(400).json({ error: 'Key required' });
    try {
        await sql`UPDATE keys SET revoked = true WHERE key_string = ${key}`;
        res.json({ success: true });
    } catch (e) {
        res.status(500).json({ error: e.message });
    }
});

// Delete a key (admin only)
app.post('/admin/delete', verifyAdmin, async (req, res) => {
    const { key } = req.body;
    if (!key) return res.status(400).json({ error: 'Key required' });
    try {
        await sql`DELETE FROM keys WHERE key_string = ${key}`;
        res.json({ success: true });
    } catch (e) {
        res.status(500).json({ error: e.message });
    }
});

// Stats (admin only)
app.get('/admin/stats', verifyAdmin, async (req, res) => {
    try {
        const now = Date.now();
        const [total]   = await sql`SELECT COUNT(*) FROM keys`;
        const [active]  = await sql`SELECT COUNT(*) FROM keys WHERE expires_at > ${now} AND revoked = false`;
        const [expired] = await sql`SELECT COUNT(*) FROM keys WHERE expires_at < ${now}`;
        const [revoked] = await sql`SELECT COUNT(*) FROM keys WHERE revoked = true`;
        res.json({
            total:   Number(total.count),
            active:  Number(active.count),
            expired: Number(expired.count),
            revoked: Number(revoked.count)
        });
    } catch (e) {
        res.status(500).json({ error: e.message });
    }
});

// ==========================================
// PUBLIC: Verify key (called by C++ mod menu)
// ==========================================
app.get('/verify', verifyLimiter, async (req, res) => {
    const { key, hwid } = req.query;
    if (!key || !hwid) return res.json({ success: false, reason: 'Missing params' });

    try {
        const rows = await sql`SELECT * FROM keys WHERE key_string = ${key} LIMIT 1`;
        if (!rows.length) return res.json({ success: false, reason: 'Key not found' });

        const found = rows[0];
        const now   = Date.now();

        if (found.revoked)                         return res.json({ success: false, reason: 'Key revoked' });
        if (now > Number(found.expires_at))        return res.json({ success: false, reason: 'Key expired' });

        // First use: bind HWID
        if (!found.hwid) {
            await sql`UPDATE keys SET hwid = ${hwid} WHERE key_string = ${key}`;
        } else if (found.hwid !== hwid) {
            return res.json({ success: false, reason: 'Key bound to different device' });
        }

        const daysRemaining = Math.max(0, Math.ceil((Number(found.expires_at) - now) / 86400000));
        res.json({
            success:        true,
            days_remaining: daysRemaining,
            expires_at:     Number(found.expires_at),
            username:       found.username
        });
    } catch (e) {
        res.status(500).json({ success: false, reason: 'Server error' });
    }
});

// ==========================================
// FREE 1-DAY KEY GENERATION (via ad bypass)
// ==========================================

// Helper: verify HMAC-MD5 signature from C++ client
function makeSignature(hwid) {
    return crypto.createHash('md5')
        .update('hwid=' + hwid + SERVER_SECRET)
        .digest('hex');
}

const genLimiter = rateLimit({ windowMs: 24 * 60 * 60 * 1000, max: 3 }); // 3 per 24h per IP

// Step 1: User opens /?hwid=X&signature=S  → redirect through LootLabs ad
app.get('/', async (req, res, next) => {
    const { hwid, signature } = req.query;

    // No params → serve normal landing page
    if (!hwid || !signature) return next();

    // Verify signature
    if (signature !== makeSignature(hwid)) {
        return res.status(400).send('Invalid signature');
    }

    // Build callback URL that LootLabs will redirect to after ad
    const SERVER_HOST = req.protocol + '://' + req.get('host');
    const callbackUrl = `${SERVER_HOST}/generate?hwid=${encodeURIComponent(hwid)}&signature=${encodeURIComponent(signature)}`;

    // Wrap in LootLabs bypass link
    const lootlabsUrl = 'https://loot-link.com/s?' + Buffer.from(callbackUrl).toString('base64');

    res.redirect(lootlabsUrl);
});

// Step 2: LootLabs redirects here after ad completion → generate 1-day key
app.get('/generate', genLimiter, async (req, res) => {
    const { hwid, signature } = req.query;

    if (!hwid || !signature) {
        return res.status(400).send('Missing params');
    }

    if (signature !== makeSignature(hwid)) {
        return res.status(403).send('Invalid signature');
    }

    try {
        // Check if a valid free key already exists for this HWID (prevent spamming)
        const now = Date.now();
        const existing = await sql`
            SELECT key_string FROM keys
            WHERE hwid = ${hwid}
              AND revoked = false
              AND expires_at > ${now}
              AND username = 'FREE_1DAY'
            LIMIT 1
        `;

        let keyString;
        if (existing.length > 0) {
            keyString = existing[0].key_string;
        } else {
            keyString = 'FREE_' + crypto.randomBytes(6).toString('hex').toUpperCase();
            const expiresAt = now + (24 * 60 * 60 * 1000); // 1 day
            await sql`
                INSERT INTO keys (key_string, username, hwid, created_at, expires_at, days, revoked)
                VALUES (${keyString}, 'FREE_1DAY', ${hwid}, ${now}, ${expiresAt}, 1, false)
            `;
        }

        // Redirect to landing page showing the key
        res.redirect('/?generated_key=' + encodeURIComponent(keyString));
    } catch (e) {
        res.status(500).send('Server error: ' + e.message);
    }
});

// ==========================================
// Pages + static files (registered LAST so
// dynamic routes above take priority on GET /)
// ==========================================
app.get('/admin', (req, res) => res.sendFile(path.join(__dirname, 'public', 'admin.html')));
app.use(express.static(path.join(__dirname, 'public')));

// ==========================================
// Start server
// ==========================================
const PORT = process.env.PORT || 5000;
app.listen(PORT, () => console.log(`Server running on port ${PORT}`));

module.exports = app;
