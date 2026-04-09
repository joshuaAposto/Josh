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

#include "json.hpp"
using json = nlohmann::json;

static ImVec2 aim_target_screen = ImVec2(0, 0);
static std::atomic<bool> aim_target_valid{false};
static std::atomic<bool> aim_target_visible{false};  // ✅ TAMBAHKAN INI

const char* ENTITY_FIFO_PATH = "/data/data/" PACKAGE_NAME "/imgui/esync";


struct Entity {
    std::map<std::string, std::vector<float>> bones;
    std::string entity_name;
    bool is_on_screen;
    bool is_bot;
    bool is_knocked;
    bool is_visible;
    int team_id;
    bool is_alive;
    bool is_snap;
    int id;
    float is_health = 0.0f;     
    float max_health = 125.0f;  
    float is_armor = 0.0f;    
    float max_armor = 100.0f;  
};



static std::vector<Entity> entities;
static std::mutex entities_mutex;
static std::atomic<bool> entity_thread_running{false};

static bool ensure_entity_fifo() {
    struct stat st;
    
    if (stat(ENTITY_FIFO_PATH, &st) == 0) {
        if (S_ISFIFO(st.st_mode)) {
            LOGI("Using existing FIFO at %s", ENTITY_FIFO_PATH);
            return true;
        } else unlink(ENTITY_FIFO_PATH);
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
    
    while (entity_thread_running) {
        if (fd < 0) {
            fd = open(ENTITY_FIFO_PATH, O_RDONLY | O_NONBLOCK);
            
            if (fd < 0) {
                if (errno == ENXIO) { // No writer attached yet, this is normal
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        
char buffer[15536];  

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
            std::string data(buffer);
            std::stringstream ss(data);
            std::string line;
            
            
            std::vector<Entity> new_entities;
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
                    
                    entity.bones = entity_json["bones"].get<std::map<std::string, std::vector<float>>>();
                    entity.is_on_screen = entity_json["is_on_screen"];
                    entity.is_bot = entity_json["is_bot"];
                    entity.is_knocked = entity_json["is_knocked"];
                    entity.is_visible = entity_json["is_visible"];
                    entity.is_health = entity_json["is_health"];
                    entity.max_health = entity_json["max_health"];
                    entity.is_armor = entity_json["is_armor"];
                    entity.max_armor = entity_json["max_armor"];
                    entity.team_id = entity_json["team_id"];
                    entity.entity_name = entity_json["entity_name"];
                    
                    if (entity.is_on_screen) {
                        new_entities.push_back(entity);
                        entity_count++; 
                    }
                }
            }

            
            LOGI("Total entities processed: %d", entity_count);  
            
            {
                std::lock_guard<std::mutex> lock(entities_mutex);
                entities = new_entities; 
            }
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



static void update_aim_target() {
    const char* AIM_PATH = "/data/data/" PACKAGE_NAME "/imgui/aim_target";
    std::ifstream file(AIM_PATH);
    if (!file.is_open()) {
        aim_target_valid = false;
        aim_target_visible = false; // <- Reset visible juga
        return;
    }

    std::string line;
    std::getline(file, line);
    file.close();

    try {
        json j = json::parse(line);
        aim_target_screen = ImVec2(j["x"], j["y"]);
        aim_target_valid = j["valid"];
        aim_target_visible = j.value("visible", true); // <- BACA NILAI VISIBLE DI SINI
    } catch (...) {
        aim_target_valid = false;
        aim_target_visible = false;
    }
}

static void start_entity_reader() {
    if (entity_thread_running.exchange(true)) {
        LOGI("Entity reader thread already running");
        return;
    }
    
    std::thread(entity_reader_thread_func).detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

static void stop_entity_reader() {
    entity_thread_running = false;
    LOGI("Stopping entity reader thread...");
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

static std::vector<Entity> get_entities() {
    std::lock_guard<std::mutex> lock(entities_mutex);
    return entities;
}

