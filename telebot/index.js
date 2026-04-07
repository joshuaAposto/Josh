require('dotenv').config();
const TelegramBot = require('node-telegram-bot-api');
const { initDB, addVip, removeVip, getVip, listVip, daysRemaining } = require('./db');

const TOKEN = '8435692186:AAHBVwYMLpjbKOmH0Mqr61micneO89Lqn8A';
const ADMIN_IDS = [7852634111];

const bot = new TelegramBot(TOKEN, { polling: true });

function isAdmin(userId) {
    return ADMIN_IDS.includes(userId);
}

function formatDate(dt) {
    return new Date(dt).toDateString();
}

// Welcome new members
bot.on('new_chat_members', (msg) => {
    const chatId = msg.chat.id;
    msg.new_chat_members.forEach(member => {
        const name = member.first_name || 'User';
        bot.sendMessage(chatId,
            `Welcome to SACREDBS VIP, ${name}!\n\n` +
            `You have joined the official ModMenu community.\n\n` +
            `Here is what you can do:\n` +
            `/start - Get started and see bot commands\n` +
            `/tutorial - Learn how to use the mod menu\n` +
            `/vipstatus - Check your VIP access status\n\n` +
            `If you have any issues, send a message describing your problem and your phone model.`
        );
    });
});

// /start
bot.onText(/\/start/, (msg) => {
    const chatId = msg.chat.id;
    const name = msg.from.first_name || 'User';
    bot.sendMessage(chatId,
        `Hello ${name}, welcome to the SACREDBS VIP Bot.\n\n` +
        `Available commands:\n` +
        `/start - Show this message\n` +
        `/tutorial - How to use the mod menu\n` +
        `/vipstatus - Check your VIP access and days remaining\n` +
        `/troubleshoot - Fix common issues\n\n` +
        `Admin commands:\n` +
        `/addvip [user_id] [days] - Add a VIP user\n` +
        `/removevip [user_id] - Remove a VIP user\n` +
        `/listvip - List all VIP users\n` +
        `/checkvip [user_id] - Check a specific user VIP status`
    );
});

// /tutorial
bot.onText(/\/tutorial/, (msg) => {
    const chatId = msg.chat.id;
    bot.sendMessage(chatId,
        `How to use SACREDBS VIP ModMenu:\n\n` +
        `Step 1. Download and install the app from the channel.\n\n` +
        `Step 2. Launch the app directly. It opens automatically so you do not need to go to your home screen to bring it back.\n\n` +
        `Step 3. Open the game you want to use the mod menu on.\n\n` +
        `Step 4. The mod menu overlay will appear. Tap any feature to enable or disable it.\n\n` +
        `Step 5. If a feature stops working, follow these steps:\n` +
        `   - Turn on Airplane Mode\n` +
        `   - Wait 5 seconds\n` +
        `   - Turn off Airplane Mode\n` +
        `   - Reconnect to WiFi or mobile data\n` +
        `   - Launch the app again\n\n` +
        `If the problem continues, send us a message with your phone model and a description of the issue.`
    );
});

// /troubleshoot
bot.onText(/\/troubleshoot/, (msg) => {
    const chatId = msg.chat.id;
    bot.sendMessage(chatId,
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
        `- What exactly happened`
    );
});

// /vipstatus
bot.onText(/\/vipstatus/, async (msg) => {
    const chatId = msg.chat.id;
    const userId = msg.from.id;
    try {
        const user = await getVip(userId);
        if (!user) {
            return bot.sendMessage(chatId, `Your account does not have VIP access.\n\nContact an admin to get VIP access.`);
        }
        if (!user.active) {
            return bot.sendMessage(chatId,
                `Your VIP access has expired.\n` +
                `Expired on: ${formatDate(user.expires_at)}\n\n` +
                `Contact an admin to renew your access.`
            );
        }
        const days = daysRemaining(user.expires_at);
        bot.sendMessage(chatId,
            `VIP Status: Active\n` +
            `Days remaining: ${days} day(s)\n` +
            `Expires on: ${formatDate(user.expires_at)}`
        );
    } catch (err) {
        bot.sendMessage(chatId, 'Error checking VIP status. Please try again later.');
        console.error(err);
    }
});

// /addvip [user_id] [days]
bot.onText(/\/addvip (.+)/, async (msg, match) => {
    const chatId = msg.chat.id;
    const senderId = msg.from.id;

    if (!isAdmin(senderId)) {
        return bot.sendMessage(chatId, 'You do not have permission to use this command.');
    }

    const args = match[1].trim().split(/\s+/);
    if (args.length < 2) {
        return bot.sendMessage(chatId, 'Usage: /addvip [user_id] [days]');
    }

    const targetId = parseInt(args[0]);
    const days = parseInt(args[1]);

    if (isNaN(targetId) || isNaN(days) || days <= 0) {
        return bot.sendMessage(chatId, 'Invalid user ID or number of days.');
    }

    try {
        const user = await addVip(targetId, days, senderId);
        const remaining = daysRemaining(user.expires_at);
        bot.sendMessage(chatId,
            `VIP access granted.\n` +
            `User ID: ${targetId}\n` +
            `Days added: ${days}\n` +
            `Total days remaining: ${remaining}\n` +
            `Expires on: ${formatDate(user.expires_at)}`
        );
        bot.sendMessage(targetId,
            `Your VIP access has been activated.\n` +
            `Days added: ${days}\n` +
            `Total days remaining: ${remaining}\n` +
            `Expires on: ${formatDate(user.expires_at)}\n\n` +
            `Use /tutorial to learn how to use the mod menu.`
        ).catch(() => {});
    } catch (err) {
        bot.sendMessage(chatId, 'Error adding VIP. Please try again.');
        console.error(err);
    }
});

// /removevip [user_id]
bot.onText(/\/removevip (.+)/, async (msg, match) => {
    const chatId = msg.chat.id;
    const senderId = msg.from.id;

    if (!isAdmin(senderId)) {
        return bot.sendMessage(chatId, 'You do not have permission to use this command.');
    }

    const targetId = parseInt(match[1].trim());
    if (isNaN(targetId)) {
        return bot.sendMessage(chatId, 'Invalid user ID. Usage: /removevip [user_id]');
    }

    try {
        const removed = await removeVip(targetId);
        if (!removed) {
            return bot.sendMessage(chatId, `User ${targetId} is not in the VIP list.`);
        }
        bot.sendMessage(chatId, `VIP access removed for user ID: ${targetId}`);
        bot.sendMessage(targetId,
            `Your VIP access has been removed.\n\nContact an admin if you believe this is a mistake.`
        ).catch(() => {});
    } catch (err) {
        bot.sendMessage(chatId, 'Error removing VIP. Please try again.');
        console.error(err);
    }
});

// /listvip
bot.onText(/\/listvip/, async (msg) => {
    const chatId = msg.chat.id;
    const senderId = msg.from.id;

    if (!isAdmin(senderId)) {
        return bot.sendMessage(chatId, 'You do not have permission to use this command.');
    }

    try {
        const users = await listVip();
        if (users.length === 0) {
            return bot.sendMessage(chatId, 'No VIP users found.');
        }

        const active = users.filter(u => u.active);
        const expired = users.filter(u => !u.active);

        let text = `VIP Users List\n\n`;
        if (active.length > 0) {
            text += `Active (${active.length}):\n`;
            active.forEach(u => {
                text += `- ID: ${u.user_id} | ${daysRemaining(u.expires_at)} day(s) left | Expires: ${formatDate(u.expires_at)}\n`;
            });
        }
        if (expired.length > 0) {
            text += `\nExpired (${expired.length}):\n`;
            expired.forEach(u => {
                text += `- ID: ${u.user_id} | Expired: ${formatDate(u.expires_at)}\n`;
            });
        }

        bot.sendMessage(chatId, text);
    } catch (err) {
        bot.sendMessage(chatId, 'Error fetching VIP list. Please try again.');
        console.error(err);
    }
});

// /checkvip [user_id]
bot.onText(/\/checkvip (.+)/, async (msg, match) => {
    const chatId = msg.chat.id;
    const senderId = msg.from.id;

    if (!isAdmin(senderId)) {
        return bot.sendMessage(chatId, 'You do not have permission to use this command.');
    }

    const targetId = parseInt(match[1].trim());
    if (isNaN(targetId)) {
        return bot.sendMessage(chatId, 'Invalid user ID. Usage: /checkvip [user_id]');
    }

    try {
        const user = await getVip(targetId);
        if (!user) {
            return bot.sendMessage(chatId, `User ID ${targetId} does not have VIP access.`);
        }
        if (!user.active) {
            return bot.sendMessage(chatId,
                `User ID: ${targetId}\n` +
                `Status: Expired\n` +
                `Expired on: ${formatDate(user.expires_at)}`
            );
        }
        const days = daysRemaining(user.expires_at);
        bot.sendMessage(chatId,
            `User ID: ${targetId}\n` +
            `Status: Active\n` +
            `Days remaining: ${days}\n` +
            `Expires on: ${formatDate(user.expires_at)}`
        );
    } catch (err) {
        bot.sendMessage(chatId, 'Error checking user. Please try again.');
        console.error(err);
    }
});

// Start bot
initDB().then(() => {
    console.log('SACREDBS VIP Bot is running...');
}).catch(err => {
    console.error('Failed to connect to database:', err);
    process.exit(1);
});
