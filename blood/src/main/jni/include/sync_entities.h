#pragma once
#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <map>
#include <deque>
#include <unordered_map>
#include <condition_variable>
#include <memory>
#include <functional>

#include "json.hpp"

// ================= CONFIG =================
#define ENTITY_FIFO_PATH "/data/data/" PACKAGE_NAME "/imgui/esync"
#define ENTITY_BUFFER_SIZE 262144   // 256 KB buffer
#define ENTITY_MAX_BACKLOG 128      // queue max blocking
#define ENTITY_RESERVE_COUNT 128    // reserve for vector 
#define ENTITY_STALE_MS 1000        // when entity is stale
#define ENTITY_MAX_DISTANCE 200.0f  // max render distance

// ================= STRUCT =================
struct Entity {
    std::map<std::string, std::vector<float>> bones;
    bool is_on_screen     = false;
    bool is_bot           = false;
    bool is_knocked       = false;
    bool is_visible       = false;
    bool is_local_player  = false;
    bool is_alive         = true;

    std::string name      = "ROBOT";

    float health          = 125.0f;
    float armor           = 125.0f;
    float team_id         = 0;
    float distance        = 9999.0f;

    // raw values
    float is_health       = 125.0f;
    float is_armor        = 125.0f;
    float is_team_id      = 0;

    // **WEAPON SYSTEM**
    std::string weapon_name;
    int weapon_id;
    std::string weapon_type;
    float weapon_damage;
    float weapon_headshot_multiplier;
    int ammo;
    int max_ammo;

    // **WEAPON MANAGEMENT**
    static std::string GetWeaponNameFromId(int weapon_id) {
        static const std::unordered_map<int, std::string> WEAPON_MAP = {
            {1, "M4A1"}, {88, "SCAR"}, {40, "AK-47"}, {72, "KAG6"}, {91, "VSS"}, {93, "AR97"},
            {98, "AUG"}, {101, "MCX"}, {102, "FAL"}, {114, "FN2000"}, {115, "GALIL"}, {121, "QBZ95"},
            {77, "M700"}, {103, "KAR98"}, {71, "KALA"}, {122, "SVD"}, {83, "BOW"},
            {34, "ORIGIN12"}, {79, "MP155"}, {126, "M1887"},
            {2, "MP5"}, {38, "VECTOR"}, {75, "URB"}, {76, "INP9"}, {90, "P90"}, {110, "UZI"},
            {100, "MAGNUM"}, {15, "GLOCK"}, {74, "DEAGLE"}
        };
        auto it = WEAPON_MAP.find(weapon_id);
        return it != WEAPON_MAP.end() ? it->second : "None";
    }

    static std::string GetWeaponTypeFromId(int weapon_id) {
        if (weapon_id == 0) return "None";
        
        if (weapon_id == 1 || weapon_id == 88 || weapon_id == 40 || weapon_id == 72 || 
            weapon_id == 91 || weapon_id == 93 || weapon_id == 98 || weapon_id == 101 || 
            weapon_id == 102 || weapon_id == 114 || weapon_id == 115 || weapon_id == 121) {
            return "assault_rifle";
        }
        else if (weapon_id == 77 || weapon_id == 103 || weapon_id == 71 || 
                weapon_id == 122 || weapon_id == 83) {
            return "sniper";
        }
        else if (weapon_id == 34 || weapon_id == 79 || weapon_id == 126) {
            return "shotgun";
        }
        else if (weapon_id == 2 || weapon_id == 38 || weapon_id == 75 || 
                weapon_id == 76 || weapon_id == 90 || weapon_id == 110) {
            return "smg";
        }
        else if (weapon_id == 100 || weapon_id == 15 || weapon_id == 74) {
            return "pistol";
        }
        return "None";
    }

    static float GetWeaponDamageFromId(int weapon_id) {
        static const std::unordered_map<int, float> DAMAGE_MAP = {
            {1, 32.0f}, {88, 34.0f}, {40, 35.0f}, {72, 31.0f}, {91, 28.0f}, {93, 30.0f},
            {98, 33.0f}, {101, 32.5f}, {102, 36.0f}, {114, 31.5f}, {115, 34.5f}, {121, 32.8f},
            {77, 85.0f}, {103, 88.0f}, {71, 95.0f}, {122, 82.0f}, {83, 90.0f},
            {34, 22.0f}, {79, 24.0f}, {126, 26.0f},
            {2, 25.0f}, {38, 28.0f}, {75, 26.0f}, {76, 27.0f}, {90, 24.0f}, {110, 23.0f},
            {100, 60.0f}, {15, 25.0f}, {74, 62.0f}
        };
        auto it = DAMAGE_MAP.find(weapon_id);
        return it != DAMAGE_MAP.end() ? it->second : 0.0f;
    }

    static float GetWeaponHeadshotMultiplierFromId(int weapon_id) {
        static const std::unordered_map<int, float> MULTIPLIER_MAP = {
            {1, 1.4f}, {88, 1.45f}, {40, 1.5f}, {72, 1.35f}, {91, 1.8f}, {93, 1.4f},
            {98, 1.45f}, {101, 1.42f}, {102, 1.55f}, {114, 1.38f}, {115, 1.48f}, {121, 1.43f},
            {77, 2.2f}, {103, 2.3f}, {71, 2.5f}, {122, 2.1f}, {83, 2.8f},
            {34, 1.9f}, {79, 1.85f}, {126, 2.0f},
            {2, 1.3f}, {38, 1.25f}, {75, 1.28f}, {76, 1.26f}, {90, 1.22f}, {110, 1.2f},
            {100, 1.7f}, {15, 1.4f}, {74, 1.75f}
        };
        auto it = MULTIPLIER_MAP.find(weapon_id);
        return it != MULTIPLIER_MAP.end() ? it->second : 1.0f;
    }

    void UpdateWeaponInfo() {
        if (weapon_name.empty() && weapon_id > 0) {
            weapon_name = GetWeaponNameFromId(weapon_id);
        }
        if (weapon_type == "None" && weapon_id > 0) {
            weapon_type = GetWeaponTypeFromId(weapon_id);
        }
        if (weapon_damage <= 0 && weapon_id > 0) {
            weapon_damage = GetWeaponDamageFromId(weapon_id);
        }
        if (weapon_headshot_multiplier <= 1.0 && weapon_id > 0) {
            weapon_headshot_multiplier = GetWeaponHeadshotMultiplierFromId(weapon_id);
        }
    }
    
    bool GetBonePosition(const std::string& bone_name, ImVec2& out_pos) const {
        auto it = bones.find(bone_name);
        if (it != bones.end() && it->second.size() >= 2) {
            out_pos = ImVec2(it->second[0], it->second[1]);
            return true;
        }
        return false;
    }

    // last update timestamp -> prevents blink
    std::chrono::steady_clock::time_point last_seen = std::chrono::steady_clock::now();
};

// ================= BLOCKING QUEUE =================
class BlockingQueue {
public:
    BlockingQueue(size_t capacity) : capacity_(capacity) {}

    void push(std::string &&item) {
        std::unique_lock<std::mutex> lk(mtx_);
        cond_full_.wait(lk, [&]() { return !running_ || queue_.size() < capacity_; });
        if (!running_) return;
        queue_.push_back(std::move(item));
        lk.unlock();
        cond_empty_.notify_one();
    }

    bool pop(std::string &out) {
        std::unique_lock<std::mutex> lk(mtx_);
        cond_empty_.wait(lk, [&]() { return !running_ || !queue_.empty(); });
        if (queue_.empty()) return false;
        out = std::move(queue_.front());
        queue_.pop_front();
        lk.unlock();
        cond_full_.notify_one();
        return true;
    }

    void stop() {
        std::lock_guard<std::mutex> lk(mtx_);
        running_ = false;
        cond_empty_.notify_all();
        cond_full_.notify_all();
    }

    void clear() {
        std::lock_guard<std::mutex> lk(mtx_);
        queue_.clear();
    }

private:
    std::deque<std::string> queue_;
    size_t capacity_;
    std::mutex mtx_;
    std::condition_variable cond_empty_;
    std::condition_variable cond_full_;
    bool running_ = true;
};

// ================= STATE =================
static std::shared_ptr<const std::vector<Entity>> g_entities_front = std::make_shared<const std::vector<Entity>>(std::vector<Entity>{});
static std::mutex g_front_swap_mutex;

static std::unordered_map<std::string, Entity> g_entities_map;
static std::mutex g_map_mutex;

static std::atomic<bool> g_entity_thread_running{false};
static std::thread g_reader_thread;
static std::thread g_parser_thread;
static std::unique_ptr<BlockingQueue> g_backlog_queue;

// ================= FIFO HELPERS =================
static bool ensure_entity_fifo() {
    struct stat st;
    if (stat(ENTITY_FIFO_PATH, &st) == 0) {
        if (S_ISFIFO(st.st_mode)) return true;
        unlink(ENTITY_FIFO_PATH);
    }
    return mkfifo(ENTITY_FIFO_PATH, 0666) == 0;
}

// ================= PARSER =================
static void process_json_batch(const std::string &line) {
    using json = nlohmann::json;
    json arr;
    try {
        arr = json::parse(line);
    } catch (...) {
        return;
    }

    if (!arr.is_array()) return;

    auto now = std::chrono::steady_clock::now();
    std::unordered_map<std::string, Entity> local_updates;
    local_updates.reserve(arr.size() * 2);

    int idx = 0;
    for (auto &j : arr) {
        Entity e;

        e.bones           = j.value("bones", std::map<std::string, std::vector<float>>{});
        e.is_on_screen    = j.value("is_on_screen", false);
        e.is_bot          = j.value("is_bot", false);
        e.is_knocked      = j.value("is_knocked", false);
        e.is_visible      = j.value("is_visible", false);
        e.distance        = j.value("distance", 9999.f);

        e.weapon_id                  = j.value("weapon_id", 0);
        e.weapon_name                = j.value("weapon_name", "");
        e.weapon_type                = j.value("weapon_type", "pisau");
        e.weapon_damage              = j.value("weapon_damage", 0.0f);
        e.weapon_headshot_multiplier = j.value("weapon_headshot_multiplier", 1.0f);
        e.UpdateWeaponInfo();

        e.is_health = j.value("is_health", 125.f);
        e.is_armor  = j.value("is_armor", 125.f);
        e.health    = e.is_health;
        e.armor     = e.is_armor;

        if (j.contains("is_team_id"))
            e.team_id = static_cast<int>(j.value("is_team_id", 0.f));
        else
            e.team_id = j.value("team_id", 0);

        std::string rawName = j.value("name", "");
        if (!rawName.empty()) e.name = "[ " + rawName + " ]";
        else e.name = "";

        e.is_local_player = j.value("is_local_player", false);
        e.is_alive        = j.value("is_alive", true);
        e.last_seen       = now;

        std::string key;
        if (j.contains("ptr")) key = std::to_string(j.value("ptr", 0LL));
        else if (j.contains("address")) key = std::to_string(j.value("address", 0LL));
        else if (j.contains("id")) key = std::to_string(j.value("id", 0LL));
        else key = rawName + "_" + std::to_string((int)e.team_id) + "_" + std::to_string(idx);

        local_updates.emplace(std::move(key), std::move(e));
        idx++;
    }

    {
        std::lock_guard<std::mutex> lk(g_map_mutex);
        for (auto &p : local_updates) {
            const std::string &key = p.first;
            Entity &newE = const_cast<Entity&>(p.second); 
            auto it = g_entities_map.find(key);
            if (it == g_entities_map.end()) {
                g_entities_map.emplace(key, std::move(newE));
            } else {
                Entity &existing = it->second;
                existing.bones = std::move(newE.bones);
                existing.is_on_screen = newE.is_on_screen;
                existing.is_bot = newE.is_bot;
                existing.is_knocked = newE.is_knocked;
                existing.is_visible = newE.is_visible;
                existing.is_local_player = newE.is_local_player;
                existing.is_alive = newE.is_alive;
                existing.name = std::move(newE.name);
                existing.is_health = newE.is_health;
                existing.is_armor = newE.is_armor;
                existing.health = newE.health;
                existing.armor = newE.armor;
                existing.team_id = newE.team_id;
                existing.distance = newE.distance;
                existing.last_seen = newE.last_seen;
            }
        }

        auto now_tp = std::chrono::steady_clock::now();
        std::vector<std::string> to_erase;
        to_erase.reserve(8);
        for (auto &kv : g_entities_map) {
            auto age = std::chrono::duration_cast<std::chrono::milliseconds>(now_tp - kv.second.last_seen).count();
            if (age > ENTITY_STALE_MS * 5) {
                to_erase.push_back(kv.first);
            }
        }
        for (auto &k : to_erase) g_entities_map.erase(k);
    }

    {
        std::shared_ptr<std::vector<Entity>> new_vec = std::make_shared<std::vector<Entity>>();
        new_vec->reserve(std::max<size_t>(ENTITY_RESERVE_COUNT, g_entities_map.size()));
        {
            std::lock_guard<std::mutex> lk(g_map_mutex);
            for (auto &kv : g_entities_map) {
                auto age = std::chrono::duration_cast<std::chrono::milliseconds>(now - kv.second.last_seen).count();
                if (age <= ENTITY_STALE_MS) {
                    new_vec->push_back(kv.second);
                }
            }
        }
        {
            std::lock_guard<std::mutex> lk(g_front_swap_mutex);
            g_entities_front = std::static_pointer_cast<const std::vector<Entity>>(new_vec);
        }
    }
}

// ================= BACKLOG PROCESSOR =================
static void backlog_processor_loop(BlockingQueue *queue, std::atomic<bool> *running_flag) {
    std::string job;
    while (running_flag->load()) {
        bool ok = queue->pop(job);
        if (!ok) break; 
        if (!job.empty()) {
            process_json_batch(job);
        }
    }
}

// ================= READER THREAD (FIXED ASYNC IPC) =================
static void entity_reader_thread_func(BlockingQueue *queue, std::atomic<bool> *running_flag) {
    if (!ensure_entity_fifo()) return;

    int fd = -1;
    auto last_data = std::chrono::steady_clock::now();
    std::string leftover;

    while (running_flag->load()) {
        if (fd < 0) {
            // FIX: Use O_RDWR so the pipe is permanently open, preventing read loops on python restarts
            fd = open(ENTITY_FIFO_PATH, O_RDWR | O_NONBLOCK);
            if (fd < 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                continue;
            }
        }

        char buffer[ENTITY_BUFFER_SIZE];
        // Read directly instead of polling, as polling named pipes with O_RDWR is undefined on Android
        ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
        
        if (n > 0) {
            buffer[n] = '\0';
            leftover += buffer;
            last_data = std::chrono::steady_clock::now();

            size_t pos;
            while ((pos = leftover.find('\n')) != std::string::npos) {
                std::string line = leftover.substr(0, pos);
                leftover.erase(0, pos + 1);

                if (!line.empty()) {
                    queue->push(std::move(line));
                }
            }
        } else {
            // No data received
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_data).count();
            
            // If 5 seconds pass with no python data, clear the screen
            if (elapsed > 5000) {
                std::shared_ptr<std::vector<Entity>> empty_vec = std::make_shared<std::vector<Entity>>();
                {
                    std::lock_guard<std::mutex> lk(g_front_swap_mutex);
                    g_entities_front = std::static_pointer_cast<const std::vector<Entity>>(empty_vec);
                }
                last_data = now; // Reset timer to prevent constant lock spamming
            }
            
            // Sleep slightly to prevent high CPU usage while waiting for python data
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    }

    if (fd >= 0) close(fd);
}

// ================= API =================
static void start_entity_reader() {
    if (g_entity_thread_running.exchange(true)) return;

    g_backlog_queue = std::make_unique<BlockingQueue>(ENTITY_MAX_BACKLOG);
    g_reader_thread = std::thread(entity_reader_thread_func, g_backlog_queue.get(), &g_entity_thread_running);
    g_parser_thread = std::thread(backlog_processor_loop, g_backlog_queue.get(), &g_entity_thread_running);
}

static void stop_entity_reader() {
    if (!g_entity_thread_running.exchange(false)) return;

    if (g_backlog_queue) g_backlog_queue->stop();

    if (g_reader_thread.joinable()) g_reader_thread.join();
    if (g_parser_thread.joinable()) g_parser_thread.join();

    {
        std::lock_guard<std::mutex> lk(g_map_mutex);
        g_entities_map.clear();
    }
    if (g_backlog_queue) g_backlog_queue->clear();
    g_backlog_queue.reset();

    std::shared_ptr<std::vector<Entity>> empty_vec = std::make_shared<std::vector<Entity>>();
    {
        std::lock_guard<std::mutex> lk(g_front_swap_mutex);
        g_entities_front = std::static_pointer_cast<const std::vector<Entity>>(empty_vec);
    }
}

static std::shared_ptr<const std::vector<Entity>> get_entities_all_ref() {
    std::lock_guard<std::mutex> lk(g_front_swap_mutex);
    return g_entities_front;
}

static std::vector<Entity> get_entities_on_screen() {
    std::vector<Entity> out;
    auto front = get_entities_all_ref();
    out.reserve(front->size());
    auto now = std::chrono::steady_clock::now();
    for (auto &e : *front) {
        auto age = std::chrono::duration_cast<std::chrono::milliseconds>(now - e.last_seen).count();
        if (age < ENTITY_STALE_MS && e.is_alive && !e.is_knocked && e.is_on_screen
            && e.distance <= ENTITY_MAX_DISTANCE) {
            out.push_back(e);
        }
    }
    return out;
}

static int get_entity_count(bool include_bots = true) {
    int count = 0;
    auto front = get_entities_all_ref();
    auto now = std::chrono::steady_clock::now();
    for (auto &e : *front) {
        auto age = std::chrono::duration_cast<std::chrono::milliseconds>(now - e.last_seen).count();
        if (age > ENTITY_STALE_MS) continue;
        if (!include_bots && e.is_bot) continue;
        if (e.is_alive && !e.is_knocked && e.distance <= ENTITY_MAX_DISTANCE) { 
            count++;
        }
    }
    return count;
}

static bool is_in_game() {
    auto front = get_entities_all_ref();
    auto now = std::chrono::steady_clock::now();
    for (auto &e : *front) {
        auto age = std::chrono::duration_cast<std::chrono::milliseconds>(now - e.last_seen).count();
        if (age > ENTITY_STALE_MS) continue;
        if (!e.is_bot && !e.is_local_player) return true;
    }
    return !front->empty();
}

static void clear_entities() {
    {
        std::lock_guard<std::mutex> lk(g_map_mutex);
        g_entities_map.clear();
    }
    std::shared_ptr<std::vector<Entity>> empty_vec = std::make_shared<std::vector<Entity>>();
    {
        std::lock_guard<std::mutex> lk(g_front_swap_mutex);
        g_entities_front = std::static_pointer_cast<const std::vector<Entity>>(empty_vec);
    }
}
