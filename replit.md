# BloodStrike Mod Project

## Overview
Android NDK project that provides game modifications for BloodStrike (com.netease.newspike). Includes anti-telemetry, ESP, aimbot, magic bullet, skin unlocking, and an ImGui overlay menu.

## Architecture
- **Language**: C++ (NDK) with embedded Python script (`srcpy.cpp`)
- **Hooking**: Dobby, And64InlineHook, xhook, KittyMemory, BNM (By-Name-Modding for Il2Cpp)
- **UI**: Dear ImGui overlay injected via eglSwapBuffers hook
- **Communication**: Named pipes (FIFOs) for sync between ImGui menu and Python game hooks

## Key Files
- `src/main/jni/include/cpp/srcpy.cpp` — Embedded Python script containing all game logic hooks (ESP, aimbot, bullet tracking, skin hack, etc.)
- `src/main/jni/main.cpp` — Native entry point, thread initialization, hook installation
- `src/main/jni/AntiTelemetry.cpp` — Telemetry/anti-cheat domain blocking
- `src/main/jni/NetworkHooks.cpp` — PLT/GOT network function hooks

## Entity Feature Initialization
Entity-based features (ESP, aimbot, magic bullet) use a two-part initialization system:
1. **Background monitor thread** (`_entity_monitor`) polls every 3s checking if `Space._instance` has entities and `StoryTick._instance` exists
2. **Main-thread registration** via `StoryTick.OnUpdate` hook — when the monitor sets `_pending_registration`, the next game tick registers all update functions (`EspUpdate`, `AIMUpdate`) with `StoryTick`
3. Registration is keyed on both `Space` and `StoryTick` identity, so it auto-re-registers when either changes (new match, reconnect, etc.)
