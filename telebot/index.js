require('dotenv').config();
const TelegramBot = require('node-telegram-bot-api');
const fs = require('fs');
const path = require('path');

const TOKEN = process.env.BOT_TOKEN;
const ADMIN_IDS = (process.env.ADMIN_IDS || '').split(',').map(id => parseInt(id.trim())).filter(Boolean);

if (!TOKEN) {
    console.error('ERROR: BOT_TOKEN is not set in your .env file.');
    process.exit(1);
}

const bot = new TelegramBot(TOKEN, { polling: true });

const VIP_FILE = path.join(__dirname, 'data', 'vip_users.json');

function loadVipUsers() {
    try {
        const raw = fs.readFileSync(VIP_FILE, 'utf8');
        return JSON.parse(raw);
    } catch {
        return {};
    }
}

function saveVipUsers(data) {
    fs.writeFileSync(VIP_FILE, JSON.stringify(data, null, 2));
}

function isAdmin(userId) {
    return ADMIN_IDS.includes(userId);
}

function isVip(userId) {
    const users = loadVipUsers();
    const user = users[userId];
    if (!user) return false;
    const now = Date.now();
    return user.expiresAt > now;
}

function getDaysRemaining(userId) {
    const users = loadVipUsers();
    const user = users[userId];
    if (!user) return 0;
    const now = Date.now();
    const diff = user.expiresAt - now;
    if (diff <= 0) return 0;
    return Math.ceil(diff / (1000 * 60 * 60 * 24));
}

function formatDate(timestamp) {
    return new Date(timestamp).toDateString();
}

// Welcome new members
bot.on('new_chat_members', (msg) => {
    const chatId = msg.chat.id;
    const newMembers = msg.new_chat_members;

    newMembers.forEach(member => {
        const name = member.first_name || 'User';
        const welcomeMsg =
            `Welcome to SACREDBS VIP, ${name}!\n\n` +
            `You have joined the official ModMenu community.\n\n` +
            `Here is what you can do:\n` +
            `/start - Get started and see bot commands\n` +
            `/tutorial - Learn how to use the mod menu\n` +
            `/vipstatus - Check your VIP access status\n\n` +
            `If you have any issues, send a message describing your problem and your phone model.`;

        bot.sendMessage(chatId, welcomeMsg);
    });
});

// /start command
bot.onText(/\/start/, (msg) => {
    const chatId = msg.chat.id;
    const name = msg.from.first_name || 'User';

    const text =
        `Hello ${name}, welcome to the SACREDBS VIP Bot.\n\n` +
        `Available commands:\n` +
        `/start - Show this message\n` +
        `/tutorial - How to use the mod menu\n` +
        `/vipstatus - Check your VIP access and days remaining\n` +
        `/troubleshoot - Fix common issues\n\n` +
        `Admin commands:\n` +
        `/addvip [user_id] [days] - Add a VIP user\n` +
        `/removevip [user_id] - Remove a VIP user\n` +
        `/listvip - List all active VIP users\n` +
        `/checkvip [user_id] - Check a specific user's VIP status`;

    bot.sendMessage(chatId, text);
});

// /tutorial command
bot.onText(/\/tutorial/, (msg) => {
    const chatId = msg.chat.id;

    const text =
        `How to use SACREDBS VIP ModMenu:\n\n` +
        `Step 1. Download and install the app from the channel.\n\n` +
        `Step 2. Launch the app directly. You do not need to go to your home screen to bring it back. It opens automatically.\n\n` +
        `Step 3. Open the game you want to use the mod menu on.\n\n` +
        `Step 4. The mod menu overlay will appear. Tap any feature to enable or disable it.\n\n` +
        `Step 5. If a feature stops working, try the following:\n` +
        `   - Turn on Airplane Mode\n` +
        `   - Wait 5 seconds\n` +
        `   - Turn off Airplane Mode\n` +
        `   - Reconnect to WiFi or mobile data\n` +
        `   - Launch the app again\n\n` +
        `If the problem continues after these steps, send us a message with your phone model and a description of the issue.`;

    bot.sendMessage(chatId, text);
});

// /troubleshoot command
bot.onText(/\/troubleshoot/, (msg) => {
    const chatId = msg.chat.id;

    const text =
        `Troubleshooting - If the cheat is not working:\n\n` +
        `1. Turn on Airplane Mode\n` +
        `2. Wait 5 seconds\n` +
        `3. Turn off Airplane Mode\n` +
        `4. Reconnect to your WiFi or mobile data\n` +
        `5. Launch the app again and try\n\n` +
        `This resolves most connection-related issues.\n\n` +
        `If the problem continues, send us a message with:\n` +
        `- Your phone model\n` +
        `- What you were doing\n` +
        `- What exactly happened`;

    bot.sendMessage(chatId, text);
});

// /vipstatus command
bot.onText(/\/vipstatus/, (msg) => {
    const chatId = msg.chat.id;
    const userId = msg.from.id;

    const users = loadVipUsers();
    const user = users[userId];

    if (!user) {
        bot.sendMessage(chatId, `Your account does not have VIP access.\n\nContact an admin to get VIP access.`);
        return;
    }

    const now = Date.now();
    if (user.expiresAt <= now) {
        bot.sendMessage(chatId, `Your VIP access has expired.\n\nExpired on: ${formatDate(user.expiresAt)}\n\nContact an admin to renew your access.`);
        return;
    }

    const days = getDaysRemaining(userId);
    bot.sendMessage(chatId,
        `VIP Status: Active\n` +
        `Days remaining: ${days} day(s)\n` +
        `Expires on: ${formatDate(user.expiresAt)}`
    );
});

// /addvip [user_id] [days] - Admin only
bot.onText(/\/addvip (.+)/, (msg, match) => {
    const chatId = msg.chat.id;
    const senderId = msg.from.id;

    if (!isAdmin(senderId)) {
        bot.sendMessage(chatId, 'You do not have permission to use this command.');
        return;
    }

    const args = match[1].trim().split(/\s+/);
    if (args.length < 2) {
        bot.sendMessage(chatId, 'Usage: /addvip [user_id] [days]');
        return;
    }

    const targetId = parseInt(args[0]);
    const days = parseInt(args[1]);

    if (isNaN(targetId) || isNaN(days) || days <= 0) {
        bot.sendMessage(chatId, 'Invalid user ID or number of days. Usage: /addvip [user_id] [days]');
        return;
    }

    const users = loadVipUsers();
    const now = Date.now();
    const existingExpiry = users[targetId] && users[targetId].expiresAt > now
        ? users[targetId].expiresAt
        : now;

    users[targetId] = {
        userId: targetId,
        expiresAt: existingExpiry + days * 24 * 60 * 60 * 1000,
        addedBy: senderId,
        addedAt: now
    };

    saveVipUsers(users);

    const remaining = Math.ceil((users[targetId].expiresAt - now) / (1000 * 60 * 60 * 24));
    bot.sendMessage(chatId,
        `VIP access granted.\n` +
        `User ID: ${targetId}\n` +
        `Days added: ${days}\n` +
        `Total days remaining: ${remaining}\n` +
        `Expires on: ${formatDate(users[targetId].expiresAt)}`
    );

    // Notify the user
    bot.sendMessage(targetId,
        `Your VIP access has been activated.\n` +
        `Days added: ${days}\n` +
        `Total days remaining: ${remaining}\n` +
        `Expires on: ${formatDate(users[targetId].expiresAt)}\n\n` +
        `Use /tutorial to learn how to use the mod menu.`
    ).catch(() => {});
});

// /removevip [user_id] - Admin only
bot.onText(/\/removevip (.+)/, (msg, match) => {
    const chatId = msg.chat.id;
    const senderId = msg.from.id;

    if (!isAdmin(senderId)) {
        bot.sendMessage(chatId, 'You do not have permission to use this command.');
        return;
    }

    const targetId = parseInt(match[1].trim());
    if (isNaN(targetId)) {
        bot.sendMessage(chatId, 'Invalid user ID. Usage: /removevip [user_id]');
        return;
    }

    const users = loadVipUsers();
    if (!users[targetId]) {
        bot.sendMessage(chatId, `User ${targetId} is not in the VIP list.`);
        return;
    }

    delete users[targetId];
    saveVipUsers(users);

    bot.sendMessage(chatId, `VIP access removed for user ID: ${targetId}`);

    bot.sendMessage(targetId,
        `Your VIP access has been removed.\n\nContact an admin if you believe this is a mistake.`
    ).catch(() => {});
});

// /listvip - Admin only
bot.onText(/\/listvip/, (msg) => {
    const chatId = msg.chat.id;
    const senderId = msg.from.id;

    if (!isAdmin(senderId)) {
        bot.sendMessage(chatId, 'You do not have permission to use this command.');
        return;
    }

    const users = loadVipUsers();
    const now = Date.now();
    const entries = Object.values(users);

    if (entries.length === 0) {
        bot.sendMessage(chatId, 'No VIP users found.');
        return;
    }

    const active = entries.filter(u => u.expiresAt > now);
    const expired = entries.filter(u => u.expiresAt <= now);

    let text = `VIP Users List\n\n`;

    if (active.length > 0) {
        text += `Active (${active.length}):\n`;
        active.forEach(u => {
            const days = Math.ceil((u.expiresAt - now) / (1000 * 60 * 60 * 24));
            text += `- ID: ${u.userId} | ${days} day(s) left | Expires: ${formatDate(u.expiresAt)}\n`;
        });
    }

    if (expired.length > 0) {
        text += `\nExpired (${expired.length}):\n`;
        expired.forEach(u => {
            text += `- ID: ${u.userId} | Expired: ${formatDate(u.expiresAt)}\n`;
        });
    }

    bot.sendMessage(chatId, text);
});

// /checkvip [user_id] - Admin only
bot.onText(/\/checkvip (.+)/, (msg, match) => {
    const chatId = msg.chat.id;
    const senderId = msg.from.id;

    if (!isAdmin(senderId)) {
        bot.sendMessage(chatId, 'You do not have permission to use this command.');
        return;
    }

    const targetId = parseInt(match[1].trim());
    if (isNaN(targetId)) {
        bot.sendMessage(chatId, 'Invalid user ID. Usage: /checkvip [user_id]');
        return;
    }

    const users = loadVipUsers();
    const user = users[targetId];

    if (!user) {
        bot.sendMessage(chatId, `User ID ${targetId} does not have VIP access.`);
        return;
    }

    const now = Date.now();
    if (user.expiresAt <= now) {
        bot.sendMessage(chatId,
            `User ID: ${targetId}\n` +
            `Status: Expired\n` +
            `Expired on: ${formatDate(user.expiresAt)}`
        );
        return;
    }

    const days = Math.ceil((user.expiresAt - now) / (1000 * 60 * 60 * 24));
    bot.sendMessage(chatId,
        `User ID: ${targetId}\n` +
        `Status: Active\n` +
        `Days remaining: ${days}\n` +
        `Expires on: ${formatDate(user.expiresAt)}`
    );
});

console.log('SACREDBS VIP Bot is running...');
