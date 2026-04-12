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
- `bss/src/main/jni/AntiTelemetry.cpp` — Telemetry/anti-cheat domain blocking (50+ domains)
- `bss/src/main/jni/NetworkHooks.cpp` — PLT/GOT network function hooks (70+ blocked IPs)
- `bss/src/main/jni/AntiCheatUltra.h` — Advanced anti-cheat protection layer (9 extra hooks: openat, readlinkat, inotify, sendmsg, recvmsg, kill, timer_create, clock_gettime, read)
- `bss/src/main/jni/AntiBan.h` — Ban payload blocking (73 keywords, 30 ban response patterns, 5 hooks: send/sendto/recv/recvfrom/write)
- `bss/src/main/jni/AntiDetection.cpp` — Root/framework hiding (53 root paths, 31 framework paths, 28 blocked security libs)
- `bss/src/main/jni/FireWall.h` — Domain-level firewall with Dobby hooks (150+ blocked domains)
- `bss/src/main/jni/BYPASS.h` — DNS-level domain redirection

## Anti-Cheat Protection Layers (10 layers)
1. **Anti-Telemetry** — Domain-based blocking for telemetry/analytics/tracking
2. **Network Hooks** — connect/dns/getaddrinfo interception
3. **Anti-Ban** — 73 ban keywords blocked outgoing, 30 ban commands suppressed incoming, write hook for unix sockets
4. **Anti-Detection** — Root path hiding, framework hiding, security lib blocking, memory region protection, system property spoofing
5. **Ultra Guard** — openat/readlinkat/inotify/sendmsg/recvmsg/kill/timer_create/clock_gettime/read hooks
6. **Firewall** — Domain + IP blacklist (70+ IPs, 150+ domains)
7. **Anti-Debug** — ptrace protection + background watchdog thread
8. **Timing Protection** — clock_gettime spoofing to prevent timing analysis
9. **Memory Protection** — Hook trampoline hiding, memory region protection
10. **Thread Evasion** — Thread scan evasion, thread name spoofing

## Entity Feature Initialization
Entity-based features (ESP, aimbot, magic bullet) use a three-layer initialization system to ensure they activate automatically when a match starts:

1. **Login-time registration** — `TryAutoEnterGame` hook attempts initial registration via `_register_updates()` when entering the game from login screen
2. **Space.__init__ hook** — Detects when a new match/space is created and spawns a delayed re-registration thread that polls for up to 30 seconds waiting for the new `StoryTick` instance
3. **Background entity watcher** — A persistent fallback thread (`_space_entity_watcher`) polls every 3 seconds, checking if `StoryTick` instance has changed and entities exist, then re-registers updates

Registration is protected by:
- `threading.Lock` (`_register_lock`) to prevent duplicate registration from concurrent threads
- `_last_story_tick_id` tracking to only register once per StoryTick instance (using Python object identity)
- `_delayed_register_active` flag to prevent overlapping delayed-register threads
