# BloodStrike Mod Project

## Overview
Android NDK project that provides game modifications for BloodStrike (com.netease.newspike). Includes anti-telemetry, ESP, aimbot, magic bullet, skin unlocking, and an ImGui overlay menu.

## Architecture
- **Language**: C++ (NDK) with embedded Python script (`srcpy.cpp`)
- **Hooking**: Dobby, And64InlineHook, xhook, KittyMemory, BNM (By-Name-Modding for Il2Cpp)
- **UI**: Dear ImGui overlay injected via eglSwapBuffers hook
- **Communication**: Named pipes (FIFOs) for sync between ImGui menu and Python game hooks

## Key Directories
- `bss/` — Primary build module with the active source code
- `src/` — Secondary/shared source directory

## Key Files
- `bss/src/main/jni/include/cpp/srcpy.cpp` — Embedded Python script containing all game logic hooks (ESP, aimbot, bullet tracking, skin hack, etc.)
- `bss/src/main/jni/main.cpp` — Native entry point, thread initialization, hook installation
- `bss/src/main/jni/AntiTelemetry.cpp` — Telemetry/anti-cheat domain blocking
- `bss/src/main/jni/NetworkHooks.cpp` — PLT/GOT network function hooks

## Entity Feature Initialization
Entity-based features (ESP, aimbot, magic bullet) use a three-layer initialization system to ensure they activate automatically when a match starts:

1. **Login-time registration** — `TryAutoEnterGame` hook attempts initial registration via `_register_updates()` when entering the game from login screen
2. **Space.__init__ hook** — Detects when a new match/space is created and spawns a delayed re-registration thread that polls for up to 30 seconds waiting for the new `StoryTick` instance
3. **Background entity watcher** — A persistent fallback thread (`_space_entity_watcher`) polls every 3 seconds, checking if `StoryTick` instance has changed and entities exist, then re-registers updates

Registration is protected by:
- `threading.Lock` (`_register_lock`) to prevent duplicate registration from concurrent threads
- `_last_story_tick_id` tracking to only register once per StoryTick instance (using Python object identity)
- `_delayed_register_active` flag to prevent overlapping delayed-register threads
