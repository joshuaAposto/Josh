#pragma once
#include "sync_entities.h"
//#include "imgui.h"
#include <unordered_set>
#include <cctype>
#include <cmath>

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

// Deteksi granat via id atau nama "GRENADE"
static inline bool IsGrenadeEntity(const Entity& e, const std::string& rawNameMaybe = "") {
    switch (e.id) { // id dari catatanmu
        case 3: case 4: case 7: case 12: case 81:
            return true;
    }
    auto normalize = [](std::string s) {
        std::string t; t.reserve(s.size());
        for (unsigned char c : s) if (std::isalpha(c)) t.push_back((char)std::toupper(c));
        return t;
    };
    std::string n = !rawNameMaybe.empty() ? normalize(rawNameMaybe) : normalize(e.name);
    return n.find("GRENADE") != std::string::npos;
}
