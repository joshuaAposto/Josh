#pragma once

#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <poll.h>

#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <sstream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <cctype>
#include <cmath>

#include "imgui.h"   // untuk ImVec2
#include "json.hpp"
using json = nlohmann::ordered_json;

const char* ENTITY_FIFO_PATH = "/data/data/" PACKAGE_NAME "/imgui/esync";

// HAPUS fungsi global UpdateWeaponInfo() yang rekursif—tidak diperlukan.


struct Entity {
    std::map<std::string, std::vector<float>> bones;

    // Status umum
    int         id = 0;          // base entity/class id
    std::string raw_name;        // nama mentah dari JSON (untuk deteksi)
    std::string name;            // nama untuk display (pakai bracket)
    bool        is_on_screen = false;
    bool        is_bot       = false;
    bool        is_knocked   = false;
    bool        is_visible   = false;

    float distance = 0.0f;

    // Data dari memory
    float health = 123.0f;
    float armor  = 125.0f;

    // Data olahan untuk ESP
    float is_health = 123.0f;
    float is_armor  = 125.0f;

    // Tim dan status musuh
    int  team_id  = 0;
    bool is_enemy = false;

    // **WEAPON SYSTEM**
    std::string weapon_name;
    int         weapon_id = 0;
    std::string weapon_type = "None";
    float       weapon_damage = 0.0f;
    float       weapon_headshot_multiplier = 1.0f;
    int         ammo = 0;
    int         max_ammo = 0;

    // **WEAPON MANAGEMENT** (static helpers)
    static std::string GetWeaponNameFromId(int weapon_id) {
        static const std::unordered_map<int, std::string> WEAPON_MAP = {
            {1,"M4A1"},{88,"SCAR"},{40,"AK-47"},{72,"KAG6"},{91,"VSS"},{93,"AR97"},
            {98,"AUG"},{101,"MCX"},{102,"FAL"},{114,"FN2000"},{115,"GALIL"},{121,"QBZ95"},
            {77,"M700"},{103,"KAR98"},{71,"KALA"},{122,"SVD"},{83,"BOW"},
            {34,"ORIGIN12"},{79,"MP155"},{126,"M1887"},
            {2,"MP5"},{38,"VECTOR"},{75,"URB"},{76,"INP9"},{90,"P90"},{110,"UZI"},
            {100,"MAGNUM"},{15,"GLOCK"},{74,"DEAGLE"}
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
        } else if (weapon_id == 77 || weapon_id == 103 || weapon_id == 71 ||
                   weapon_id == 122 || weapon_id == 83) {
            return "sniper";
        } else if (weapon_id == 34 || weapon_id == 79 || weapon_id == 126) {
            return "shotgun";
        } else if (weapon_id == 2 || weapon_id == 38 || weapon_id == 75 ||
                   weapon_id == 76 || weapon_id == 90 || weapon_id == 110) {
            return "smg";
        } else if (weapon_id == 100 || weapon_id == 15 || weapon_id == 74) {
            return "pistol";
        }
        return "None";
    }

    static float GetWeaponDamageFromId(int weapon_id) {
        static const std::unordered_map<int, float> DAMAGE_MAP = {
            {1,32.0f},{88,34.0f},{40,35.0f},{72,31.0f},{91,28.0f},{93,30.0f},
            {98,33.0f},{101,32.5f},{102,36.0f},{114,31.5f},{115,34.5f},{121,32.8f},
            {77,85.0f},{103,88.0f},{71,95.0f},{122,82.0f},{83,90.0f},
            {34,22.0f},{79,24.0f},{126,26.0f},
            {2,25.0f},{38,28.0f},{75,26.0f},{76,27.0f},{90,24.0f},{110,23.0f},
            {100,60.0f},{15,25.0f},{74,62.0f}
        };
        auto it = DAMAGE_MAP.find(weapon_id);
        return it != DAMAGE_MAP.end() ? it->second : 0.0f;
    }

    static float GetWeaponHeadshotMultiplierFromId(int weapon_id) {
        static const std::unordered_map<int, float> MULTIPLIER_MAP = {
            {1,1.4f},{88,1.45f},{40,1.5f},{72,1.35f},{91,1.8f},{93,1.4f},
            {98,1.45f},{101,1.42f},{102,1.55f},{114,1.38f},{115,1.48f},{121,1.43f},
            {77,2.2f},{103,2.3f},{71,2.5f},{122,2.1f},{83,2.8f},
            {34,1.9f},{79,1.85f},{126,2.0f},
            {2,1.3f},{38,1.25f},{75,1.28f},{76,1.26f},{90,1.22f},{110,1.2f},
            {100,1.7f},{15,1.4f},{74,1.75f}
        };
        auto it = MULTIPLIER_MAP.find(weapon_id);
        return it != MULTIPLIER_MAP.end() ? it->second : 1.0f;
    }

    void UpdateWeaponInfo() {
        if (weapon_id == 0 && !weapon_name.empty()) {
            // Bisa ditambah reverse-mapping jika perlu
            LOGI("Weapon: %s (ID: %d, Type: %s)",
                 weapon_name.c_str(), weapon_id, weapon_type.c_str());
        }
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
};


// ===== Helpers untuk grenade alert =====

// Ambil posisi layar dari bone yang tersedia
static inline bool GetAnyEntityScreenPos(const Entity& e, ImVec2& out) {
    static const char* kPref[] = {"origin","pos","position","pelvis","spine","spine1","head"};
    for (auto* key : kPref) {
        if (e.GetBonePosition(key, out)) return true;
    }
    if (!e.bones.empty()) {
        const auto& kv = *e.bones.begin();
        if (kv.second.size() >= 2) {
            out = ImVec2(kv.second[0], kv.second[1]);
            return true;
        }
    }
    return false;
}

// Deteksi granat via base id (3,4,7,12,81) atau fallback nama "GRENADE"
static inline bool IsGrenadeEntity(const Entity& e) {
    switch (e.id) {
        case 3: case 4: case 7: case 12: case 81:
            return true;
    }
    auto norm = [](std::string s) {
        std::string t; t.reserve(s.size());
        for (unsigned char c: s)
            if (std::isalpha(c))
                t.push_back((char)std::toupper(c));
        return t;
    };
    std::string n = !e.raw_name.empty() ? norm(e.raw_name) : norm(e.name);
    return n.find("GRENADE") != std::string::npos;
}


static std::vector<Entity> entities;
static std::mutex         entities_mutex;
static std::atomic<bool>  entity_thread_running{false};

static bool ensure_entity_fifo() {
    struct stat st;
    if (stat(ENTITY_FIFO_PATH, &st) == 0) {
        if (S_ISFIFO(st.st_mode)) {
            LOGI("Using existing FIFO at %s", ENTITY_FIFO_PATH);
            return true;
        } else {
            unlink(ENTITY_FIFO_PATH);
        }
    }
    if (mkfifo(ENTITY_FIFO_PATH, 0666) != 0) {
        LOGI("Failed to create entity FIFO: %s", strerror(errno));
        return false;
    }
    LOGI("Entity FIFO created at %s", ENTITY_FIFO_PATH);
    return true;
}

static void entity_reader_thread_func() {
    LOGI("Entity reader thread started");

    if (!ensure_entity_fifo()) {
        LOGI("Failed to create entity FIFO, thread exiting");
        return;
    }

    int fd = -1;
    char buffer[131072]; // 128 KB

    while (entity_thread_running) {
        if (fd < 0) {
            fd = open(ENTITY_FIFO_PATH, O_RDONLY | O_NONBLOCK);
            if (fd < 0) {
                if (errno == ENXIO) { // No writer attached yet
                    std::this_thread::sleep_for(std::chrono::milliseconds(0));
                    continue;
                } else {
                    LOGI("Error opening entity FIFO: %s", strerror(errno));
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    continue;
                }
            }
        }

        struct pollfd pfd = {0};
        pfd.fd = fd;
        pfd.events = POLLIN;

        int poll_result = poll(&pfd, 1, 100);
        if (poll_result < 0) {
            LOGI("Poll error on entity FIFO: %s", strerror(errno));
            close(fd);
            fd = -1;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        } else if (poll_result == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(0));
            continue;
        }

        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            if (errno != EAGAIN) {
                LOGI("Error reading from entity FIFO: %s", strerror(errno));
                close(fd);
                fd = -1;
            }
            continue;
        } else if (bytes_read == 0) {
            close(fd);
            fd = -1;
            continue;
        }

        buffer[bytes_read] = '\0';

        try {
            std::vector<Entity> new_entities;

            std::string data(buffer);
            std::stringstream ss(data);
            std::string line;
            int entity_count = 0;

            while (std::getline(ss, line)) {
                if (line.empty()) continue;

                json entities_json = json::parse(line);
                if (!entities_json.is_array()) {
                    LOGI("Error: Expected JSON array of entities");
                    continue;
                }

                for (const auto& entity_json : entities_json) {
                    Entity entity;

                    // Ambil bones dan flag utama
                    entity.bones        = entity_json["bones"].get<std::map<std::string, std::vector<float>>>();
                    entity.is_on_screen = entity_json["is_on_screen"];
                    entity.is_bot       = entity_json["is_bot"];
                    entity.is_knocked   = entity_json["is_knocked"];
                    entity.is_visible   = entity_json["is_visible"];
                    entity.distance     = entity_json["distance"];

                    // ID + nama
                    entity.id       = entity_json.value("id", 0);
                    entity.raw_name = entity_json.value("name", "");
                    if (!entity.raw_name.empty()) {
                        entity.name = "[ " + entity.raw_name + " ]";
                    } else {
                        entity.name = "";
                    }

                    // Weapon
                    entity.weapon_id                  = entity_json.value("weapon_id", 0);
                    entity.weapon_name                = entity_json.value("weapon_name", "");
                    entity.weapon_type                = entity_json.value("weapon_type", "pisau");
                    entity.weapon_damage              = entity_json.value("weapon_damage", 0.0f);
                    entity.weapon_headshot_multiplier = entity_json.value("weapon_headshot_multiplier", 1.0f);
                    entity.UpdateWeaponInfo();

                    // Health dan Armor
                    if (entity_json.contains("is_health") && entity_json["is_health"].is_number()) {
                        entity.is_health = entity_json["is_health"].get<float>();
                    }
                    if (entity_json.contains("is_armor") && entity_json["is_armor"].is_number()) {
                        entity.is_armor = entity_json["is_armor"].get<float>();
                    }

                    // Team ID (cek is_team_id dulu baru team_id)
                    if (entity_json.contains("is_team_id") && entity_json["is_team_id"].is_number()) {
                        entity.team_id = static_cast<int>(entity_json["is_team_id"].get<float>());
                    } else if (entity_json.contains("team_id") && entity_json["team_id"].is_number()) {
                        entity.team_id = entity_json["team_id"].get<int>();
                    } else {
                        entity.team_id = 0;
                    }

                    new_entities.push_back(entity);
                }
            }

            // Atomic update of entities vector
            {
                std::lock_guard<std::mutex> lock(entities_mutex);
                entities = std::move(new_entities);
            }
            entity_count++;
        } catch (const std::exception& e) {
            LOGI("Error parsing entity JSON: %s", e.what());
        }
    }

    if (fd >= 0) {
        close(fd);
        fd = -1;
    }

    LOGI("Entity reader thread stopped");
}

static void start_entity_reader() {
    if (entity_thread_running.exchange(true)) {
        LOGI("Entity reader thread already running");
        return;
    }
    std::thread(entity_reader_thread_func).detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(0));
}

static void stop_entity_reader() {
    entity_thread_running = false;
    LOGI("Stopping entity reader thread...");
    std::this_thread::sleep_for(std::chrono::milliseconds(0));
}

static std::vector<Entity> get_entities() {
    std::lock_guard<std::mutex> lock(entities_mutex);
    return entities;
}
