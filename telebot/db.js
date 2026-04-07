const { Pool } = require('pg');

const pool = new Pool({
    connectionString: process.env.DATABASE_URL,
    ssl: { rejectUnauthorized: false }
});

async function initDB() {
    await pool.query(`
        CREATE TABLE IF NOT EXISTS vip_users (
            user_id BIGINT PRIMARY KEY,
            expires_at TIMESTAMPTZ NOT NULL,
            added_by BIGINT,
            added_at TIMESTAMPTZ DEFAULT NOW()
        )
    `);
    console.log('Database ready.');
}

async function addVip(userId, days, addedBy) {
    const result = await pool.query(
        `INSERT INTO vip_users (user_id, expires_at, added_by, added_at)
         VALUES ($1, NOW() + ($2 || ' days')::INTERVAL, $3, NOW())
         ON CONFLICT (user_id) DO UPDATE
         SET expires_at = GREATEST(vip_users.expires_at, NOW()) + ($2 || ' days')::INTERVAL,
             added_by = $3,
             added_at = NOW()
         RETURNING *`,
        [userId, days, addedBy]
    );
    return result.rows[0];
}

async function removeVip(userId) {
    const result = await pool.query(
        `DELETE FROM vip_users WHERE user_id = $1 RETURNING *`,
        [userId]
    );
    return result.rows[0];
}

async function getVip(userId) {
    const result = await pool.query(
        `SELECT *, expires_at > NOW() AS active FROM vip_users WHERE user_id = $1`,
        [userId]
    );
    return result.rows[0];
}

async function listVip() {
    const result = await pool.query(
        `SELECT *, expires_at > NOW() AS active FROM vip_users ORDER BY expires_at DESC`
    );
    return result.rows;
}

function daysRemaining(expiresAt) {
    const diff = new Date(expiresAt) - new Date();
    if (diff <= 0) return 0;
    return Math.ceil(diff / (1000 * 60 * 60 * 24));
}

module.exports = { initDB, addVip, removeVip, getVip, listVip, daysRemaining };
