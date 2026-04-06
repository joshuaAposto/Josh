# SacRedBS VIP — Bloodstrike Android Mod Menu

## Project Overview
Android ImGui mod menu for Bloodstrike (`com.netease.newspike`) with a backend VIP key management system deployable to Vercel.

---

## Architecture

### C++ Mod Menu (`src/main/jni/`)
- **`draw.h`** — Main rendering logic, key system UI, login window, all tab content
- **`main.cpp`** — Hook setup, JVM init, ANativeWindow hooks
- **`include/cpp/srcpy.cpp`** — Python injection logic (reads offset from `libGame + 0x471A918`)
- **`include/sync_with_py.h`** — Feature sync between C++ and Python interpreter

### Backend Key System (`key/`)
- **`server.js`** — Express + MongoDB backend (runs on Vercel serverless)
- **`public/admin.html`** — Admin panel (password-gated)
- **`public/index.html`** — User-facing landing page
- **`vercel.json`** — Vercel deployment config
- **`package.json`** — Node.js dependencies

---

## Key System Design

### Admin Flow
1. Admin logs into `/admin` page using `ADMIN_PASSWORD` env var
2. Admin generates a key with a username/label and day duration (1–365 days)
3. Key is stored in MongoDB with `expiresAt` timestamp; HWID is `null` until first use

### User Flow
1. User receives key from admin/reseller
2. User pastes key into mod menu login screen
3. Mod menu calls `/verify?key=XXX&hwid=YYY`
4. On first use: HWID is bound to the key permanently
5. Response includes `days_remaining`, `expires_at`, and `username`
6. Days remaining shown in mod menu INFO tab (color-coded: green > 7d, yellow > 2d, red ≤ 2d)

### Key Format
`SACRED_XXXXXXXXXXXXXX` (SACRED_ prefix + 12 random hex chars)

---

## Environment Variables (Vercel)

| Variable | Description |
|---|---|
| `MONGO_URI` | MongoDB Atlas connection string |
| `ADMIN_PASSWORD` | Admin panel password |
| `SERVER_SECRET` | HMAC secret for future signature verification |

---

## Deployment (Vercel)

```bash
cd key
npm install
vercel --prod
```

After deploying, update the server URL in `src/main/jni/draw.h`:
```cpp
std::string url = oxorany("https://YOUR_VERCEL_URL.vercel.app/verify?key=");
```

---

## Critical C++ Notes

### Header Text / Font Rules
- **NEVER** use `dl->AddText(font, customSize, pos, col, str)` — garbles text at non-default sizes
- **ALWAYS** use `ImGui::SetWindowFontScale(X)` + `ImGui::SetCursorPos()` + `ImGui::TextColored()`
- Game font supports **UPPERCASE ASCII only**

### Push/Pop Balance
- Floating button: 2 StyleVar + 2 StyleColor
- Login window: 3 StyleVar + 6 StyleColor
- Main menu: 3 StyleVar + 12 StyleColor

### Cheat Features Note
Features (ESP, aimbot, etc.) work through Python injection at `libGame + 0x471A918`.
If the game updates, this offset will break all features. The mod menu UI and key system remain unaffected.

---

## Branding
- Product name: **SacRedBS VIP**
- Telegram: `T.ME/SACREDBS_VIP`
- Theme color: Red (`ImVec4(0.85f, 0.08f, 0.08f, 1.0f)`)
- Accent color: `ImVec4(0.95f, 0.60f, 0.10f, 1.0f)`
