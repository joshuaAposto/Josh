# SacRedBS — BloodStrike VIP Project

## Overview
BloodStrike Android mod menu with VIP key authentication system.

## Architecture

### Servers
- **key/** — Key generation server, deployed at `bskey.vercel.app` (port 5000 locally)
- **bs/** — BloodStrike landing page & key system (port 3000 locally)

Both servers share the same PostgreSQL (Neon) database.

### Workflow
`node key/server.js & node bs/server.js`
- key server → port 5000 (webview / port 80 external)
- bs server → port 3000 (external port 3000)

## Key System
- FREE keys: 24-hour, hardware-locked (HWID-bound), generated via LootLabs ad flow
- VIP keys: Admin-generated, device-independent, configurable duration
- Key format: `SACRED_XXXXXXXX`

## Config (key/server.js & bs/server.js)
All sensitive values support environment variables:
- `YOUR_DOMAIN` — Public domain for checkpoint redirects (default: `https://bskey.vercel.app`)
- `DATABASE_URL` — PostgreSQL connection string
- `LOOT_LINK_API_TOKEN` — LootLabs API token
- `SERVER_SECRET` — HMAC secret for signature verification
- `ADMIN_PASSWORD` — Admin panel password

## Pages
- `/` — Key generator (requires `?hwid=X&signature=S` from mod menu)
- `/admin` — Admin panel (password protected)
- `/generateKey` — Key generation API
- `/checkpoint` — Post-ad key issuance
- `/verify` — Key verification API (used by C++ mod menu)

## Design
Dark red theme with SacRedBS branding:
- Font: Cinzel (headings) + Rajdhani (body)
- Colors: Deep red `#8b0000` → `#cc1111`, dark background `#08090f`
- Logo: Custom SVG shield with "SBS" text
- Animated starfield background

## Files
- `key/` — Original key server (Vercel deployment)
- `bs/` — BloodStrike web app (Vercel deployment)
- `bs/src/main/jni/` — C++ mod menu source (zipped)
- `src/main/java/` — Android Java entry point
