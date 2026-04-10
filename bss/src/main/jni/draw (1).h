#include "include/hpp/imgui.h"
#include <EGL/egl.h>
#include <string>
int Width, Height;
using namespace ImGui;
using namespace std;
const char* INI_PATH = "/data/data/" PACKAGE_NAME "/imgui/.ini";
#include <vector>
#include <jni.h>
#include <android/native_activity.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <android/input.h>
#include <android/keycodes.h>
#include <android/input.h>
#include "curl/json.hpp"
#include "Jni_Stuff.h"
#include <GLES2/gl2.h>
#include "roominfofont.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include "Imgui/DreiFont.h"
#include "include/obfuscate.h"
#include "include/hpp/dlfcn.hpp"
#include "include/hpp/imgui/persistence.h"
#include "include/hpp/imgui/touch.h"
#include "include/sync_with_py.h"
#include "include/sync_entities.h"
#include "include/helpers.h"
#include "variables.h"
#include "RGB.h"
#include "Toggle.h"
#include "imgui/stb_image.h"
#include "Starcool.h"
#include "Chams.h"
#include "imgui/imgui_additional.h"
#include "Oxorany/oxorany.h"
#include "Imgui/Icon.h"
#include "Imgui/Iconcpp.h"
#include <ctime>     // Added for expiration check
#include <cstdlib>   // Added for exit(0)
#ifdef _WIN32
#include <windows.h>
#else
#include <cstdio>
#endif
#define ARGB(a, r, g, b) r << 0 | g << 8 | b << 16 | a << 24
#define ReadPointer(type, address) (*(type*)(address))
#define GetFunction(type, address, ...) ((type(*)(__VA_ARGS__))(address))
bool Zaijan = true;
#define CREATE_COLOR(r, g, b, a) new float[4]{(float)r, (float)g, (float)b, (float)a};


#include <string>
#include <cstdlib>


static GLuint menuIconTexture = 0;
static int iconW = 0, iconH = 0;

bool showMenu = false;

// Load dari PNG array
GLuint LoadTextureFromArray(unsigned char* data, int size, int* out_width, int* out_height) {
    int channels;
    unsigned char* image_data = stbi_load_from_memory(data, size, out_width, out_height, &channels, 4);
    if (!image_data) return 0;

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *out_width, *out_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image_data);
    return tex;
}


std::string generateRandomText(int length) {
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string result;
    result.reserve(length);
    for (int i = 0; i < length; ++i) {
        result += charset[rand() % (sizeof(charset) - 1)];
    }
    return result;
}


void DrawRandomTextLeftBottom() {
    static float lastUpdate = 0.0f;
    static std::string randomText = generateRandomText(10);

    float timeNow = ImGui::GetTime();
    if (timeNow - lastUpdate >= 1.0f) {
        randomText = generateRandomText(10);
        lastUpdate = timeNow;
    }

    ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    ImVec2 textPos(15.0f, screenSize.y - 50.0f); // Geser kanan 5px dari sebelumnya

    ImGui::GetForegroundDrawList()->AddText(textPos, IM_COL32(255, 0, 0, 255),
        ("Connected : " + randomText).c_str());
}


struct My_Patches {
    MemoryPatch Recoil,Magic,Assist,Smooth;
} hexPatches;
static bool recoil = false;
static bool magic = false;
static bool assist = false;
static bool smooth = false;
static bool isPlayerBotEnabled = false;



    void noMore(ImDrawList* draw) {
    if (!sync_bool["bESP"]) return;

    auto all_entities = get_entities();
    std::vector<Entity> valid_entities;

    // Pilih entitas yang valid (misalnya, yang bukan bot dan ada di layar)
    for (const Entity& e : all_entities) {
        if (!e.is_on_screen) continue;
        valid_entities.push_back(e);
    }

    // Urutkan berdasarkan jarak kamera (z value dari bone head)
    std::sort(valid_entities.begin(), valid_entities.end(), [](const Entity& a, const Entity& b) {
        float za = a.bones.at("head")[2];
        float zb = b.bones.at("head")[2];
        return za < zb;
    });

    // Batasi jumlah yang digambar
    int max_entities = 100;
    if (valid_entities.size() > max_entities) {
        valid_entities.resize(max_entities);
    }

    int players = 0, bots = 0;
    for (const Entity& e : valid_entities) {
        if (e.is_bot) bots++;
        else players++;

        if (!e.is_on_screen) continue;
        
     ImVec2 From;
        switch (sync_int["iESP_Point"]) {
            case 0: From = ImVec2(Width / 2, Height); break;
            case 1: From = ImVec2(Width / 2, Height / 2); break;
            case 2: From = ImVec2(Width / 2, 0); break;
        }

        if (sync_bool["bESP_Skeleton"]) {
            auto head = ToImVec2(e.bones.at("head"));
            auto spine = ToImVec2(e.bones.at("spine"));
            auto spine1 = ToImVec2(e.bones.at("spine1"));
            
            auto limbs_r_upperarm = ToImVec2(e.bones.at("limbs_r_upperarm"));
            auto limbs_r_forearm = ToImVec2(e.bones.at("limbs_r_forearm"));
            auto limbs_r_hand = ToImVec2(e.bones.at("limbs_r_hand"));
            
            auto limbs_r_thigh = ToImVec2(e.bones.at("limbs_r_thigh"));
            auto limbs_r_calf = ToImVec2(e.bones.at("limbs_r_calf"));
            
            auto limbs_l_upperarm = ToImVec2(e.bones.at("limbs_l_upperarm"));
            auto limbs_l_forearm = ToImVec2(e.bones.at("limbs_l_forearm"));
            auto limbs_l_hand = ToImVec2(e.bones.at("limbs_l_hand"));
            
            auto limbs_l_thigh = ToImVec2(e.bones.at("limbs_l_thigh"));
            auto limbs_l_calf = ToImVec2(e.bones.at("limbs_l_calf"));
            auto limbs_r_foot = ToImVec2(e.bones.at("limbs_r_foot"));
            auto limbs_l_foot = ToImVec2(e.bones.at("limbs_l_foot"));
            auto limbs_r_toe = ToImVec2(e.bones.at("limbs_r_toe")); // Opsional
            auto limbs_l_toe = ToImVec2(e.bones.at("limbs_l_toe")); // Opsional

            
            ImVec2 chest((limbs_r_upperarm.x + limbs_l_upperarm.x) / 2, (limbs_r_upperarm.y + limbs_l_upperarm.y) / 2);
            
            auto linecolor = e.is_visible ? colors["cESP_Skeleton"] : colors["cESP_SkeletonHidden"];
            auto thickness = sync_float["fESP_LineThickness"];
            
            draw->AddLine(head, chest, linecolor, thickness);
            draw->AddLine(chest, spine, linecolor, thickness);
            draw->AddLine(chest, limbs_r_upperarm, linecolor, thickness);
            draw->AddLine(chest, limbs_l_upperarm, linecolor, thickness);
            draw->AddLine(limbs_r_upperarm, limbs_r_forearm, linecolor, thickness);
            draw->AddLine(limbs_l_upperarm, limbs_l_forearm, linecolor, thickness);
            draw->AddLine(limbs_r_forearm, limbs_r_hand, linecolor, thickness);
            draw->AddLine(limbs_l_forearm, limbs_l_hand, linecolor, thickness);

            draw->AddLine(spine, limbs_l_thigh, linecolor, thickness);
            draw->AddLine(spine, limbs_r_thigh, linecolor, thickness);
            draw->AddLine(limbs_l_thigh, limbs_l_calf, linecolor, thickness);
            draw->AddLine(limbs_r_thigh, limbs_r_calf, linecolor, thickness);
            
            draw->AddLine(limbs_r_calf, limbs_r_foot, linecolor, thickness);
            draw->AddLine(limbs_l_calf, limbs_l_foot, linecolor, thickness);
             draw->AddLine(limbs_r_foot, limbs_r_toe, linecolor, thickness); // Jika ada
        draw->AddLine(limbs_l_foot, limbs_l_toe, linecolor, thickness); // Jika ada

        }
        
        
          ImVec2 To(ToImVec2(e.bones.at("head")));
        
if (e.is_bot) {
            if (sync_bool["bESP_LineBots"]) draw->AddLine(From, To, e.is_visible ? colors["cESP_LineBots"] : colors["cESP_LineBotsHidden"], sync_float["fESP_LineThickness"]);
        } else {
            if (sync_bool["bESP_Line"]) draw->AddLine(From, To, e.is_visible ? colors["cESP_Line"] : colors["cESP_LineHidden"], sync_float["fESP_LineThickness"]);
        }


  if ((sync_bool["bESP_TeamID"] || sync_bool["bESP_Name"] || sync_bool["bESP_Distance"] || sync_bool["bESP_Health"]) && !e.bones.empty()) {
    ImVec2 head = ToImVec2(e.bones.at("head"));
    ImVec2 foot_l = ToImVec2(e.bones.at("limbs_l_calf"));
    ImVec2 foot_r = ToImVec2(e.bones.at("limbs_r_calf"));
    ImVec2 feet = (foot_l.y > foot_r.y) ? foot_l : foot_r;

    float height = feet.y - head.y;
    float distance = 1000.0f / height;
    if (distance > 300) distance = 300;

    if (sync_bool["bESP_Distance"]) {
        std::string distanceText = std::to_string((int)distance) + "m";
        ImVec2 textSize = ImGui::CalcTextSize(distanceText.c_str());
        ImVec2 textPos = ImVec2(feet.x - textSize.x / 2, feet.y + 5.0f);
        draw->AddText(textPos, IM_COL32(0, 255, 0, 255), distanceText.c_str());
    }
    
    
    float cursorX = head.x;
    float currentY = head.y - 45.0f;

    float bar_width = 170.0f; 
    float bar_height = 35.0f;
    float startX = cursorX - bar_width / 2;


    ImVec2 barTopLeft = {startX, currentY};
    ImVec2 barBottomRight = {startX + bar_width, currentY + bar_height};
    draw->AddRectFilled(barTopLeft, barBottomRight, IM_COL32(0, 0, 0, 180), 3.0f);


    if (sync_bool["bESP_Health"]) {
        float armor_percent = e.max_armor > 0.0f ? std::min(e.is_armor / e.max_armor, 1.0f) : 0.0f;
        float health_percent = 0.0f;
        if (armor_percent < 1.0f) {
            float max_health = e.max_health > 0.0f ? e.max_health : 125.0f;
            health_percent = std::min(e.is_health / max_health, 1.0f);
        }

        float armor_width = bar_width * armor_percent;
        float health_width = bar_width * (1.0f - armor_percent) * health_percent;

        float hpBarHeight = 5.0f;
        float hpBarY = currentY + 2.0f;

        // Armor
        draw->AddRectFilled(
            {startX, hpBarY},
            {startX + armor_width, hpBarY + hpBarHeight},
            IM_COL32(138, 43, 226, 255), 2.0f);
        // HP
        if (armor_percent < 1.0f && health_percent > 0.0f) {
            ImU32 hp_color = e.is_knocked ? IM_COL32(255, 0, 0, 255) : (health_percent > 0.5f ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 255, 0, 255));
            draw->AddRectFilled(
                {startX + armor_width, hpBarY},
                {startX + armor_width + health_width, hpBarY + hpBarHeight},
                hp_color, 2.0f);
        }
    }


    if (sync_bool["bESP_TeamID"] || sync_bool["bESP_Name"]) {
        std::string fullText;
        if (sync_bool["bESP_TeamID"])
            fullText += std::to_string(e.team_id);
        if (sync_bool["bESP_TeamID"] && sync_bool["bESP_Name"])
            fullText += " | ";
        if (sync_bool["bESP_Name"])
            fullText += e.entity_name;
        if (e.is_knocked)
            fullText += "";
            

        ImVec2 textSize = ImGui::CalcTextSize(fullText.c_str());
        ImVec2 textPos = ImVec2(cursorX - textSize.x / 2.0f, currentY + (bar_height - textSize.y) / 2.0f);
        ImU32 textColor = e.is_knocked ? IM_COL32(255, 0, 0, 255) : IM_COL32(255, 255, 255, 255);
        draw->AddText(textPos, textColor, fullText.c_str());
    }

    currentY += bar_height + 4.0f;
}
   


if (sync_bool["bESP_Box"]) {
    ImVec2 head = ToImVec2(e.bones.at("head"));

    ImVec2 foot;
    bool hasFoot = false;

    if (e.bones.count("limbs_l_calf") && e.bones.count("limbs_r_calf")) {
        ImVec2 l = ToImVec2(e.bones.at("limbs_l_calf"));
        ImVec2 r = ToImVec2(e.bones.at("limbs_r_calf"));
        foot = ImVec2((l.x + r.x) / 2, (l.y + r.y) / 2);
        hasFoot = true;
    }

    if (hasFoot) {
        float height = abs(foot.y - head.y) * 2.0f;
        float width = height / 2.0f;

        ImVec2 topLeft(head.x - width / 2, head.y - height * 0.2f);
        ImVec2 bottomRight(head.x + width / 2, head.y - height * 0.2f + height);

        ImU32 color = e.is_visible ? colors["cESP_Box"] : colors["cESP_BoxHidden"];
        float thickness = sync_float["fESP_LineThickness"];

        float lineLen = width * 0.2f;

        ImVec2 tl = topLeft;
        ImVec2 br = bottomRight;
        ImVec2 tr = ImVec2(br.x, tl.y);
        ImVec2 bl = ImVec2(tl.x, br.y);

        // Top-left corner
        draw->AddLine(tl, ImVec2(tl.x + lineLen, tl.y), color, thickness); // ─
        draw->AddLine(tl, ImVec2(tl.x, tl.y + lineLen), color, thickness); // │

        // Top-right corner
        draw->AddLine(tr, ImVec2(tr.x - lineLen, tr.y), color, thickness); // ─
        draw->AddLine(tr, ImVec2(tr.x, tr.y + lineLen), color, thickness); // │

        // Bottom-left corner
        draw->AddLine(bl, ImVec2(bl.x + lineLen, bl.y), color, thickness); // ─
        draw->AddLine(bl, ImVec2(bl.x, bl.y - lineLen), color, thickness); // │

        // Bottom-right corner
        draw->AddLine(br, ImVec2(br.x - lineLen, br.y), color, thickness); // ─
        draw->AddLine(br, ImVec2(br.x, br.y - lineLen), color, thickness); // │
    }
}

    update_aim_target();

if (sync_bool["bAIM"] && aim_target_valid && aim_target_visible) {
    draw->AddLine(
        ImVec2(sync_float["fWidth"] * 0.5f, sync_float["fHeight"] * 0.5f),
        aim_target_screen,
        IM_COL32(255, 0, 0, 255),
        2.0f
    );
}


if (sync_bool["bESP_DangerCircle"] && e.is_visible < 15.0f) {
    draw->AddCircle(To, 30.0f, IM_COL32(255, 0, 0, 180), 32, 2.0f);
    draw->AddText(ImVec2(To.x - 20, To.y + 25), IM_COL32(255, 0, 0, 255), "DANGER!");
}


}



    
if (isPlayerBotEnabled && (players > 0 || bots > 0)) {
    std::string label = std::string(ICON_FA_USER) + " " + std::to_string(players) +
                        "   " +
                        std::string(ICON_FA_DESKTOP) + " " + std::to_string(bots);

    ImVec2 textSize = ImGui::CalcTextSize(label.c_str());
    float textX = (sync_float["fWidth"] - textSize.x) * 0.5f;
    float textY = 80.0f;

    // Ubah warna menjadi merah: (255, 0, 0, 255)
draw->AddText(ImVec2(textX, textY), IM_COL32(255, 0, 0, 255), label.c_str());
}


if (!sync_bool["bAIM_NoTargetHideFov"] || (players || bots)) {
    if (sync_bool["bAIM_DrawFov"]) {
    draw->AddCircle(
        ImVec2(Width / 2, Height / 2),
        sync_float["fAIM_Fov"],
        IM_COL32(255, 0, 0, 255), // Merah
        0,
        sync_float["fESP_LineThickness"]
    );
    }
    }
    }
    

   void DrawMenu(ImGuiIO &io) {
       bool need_sync = false, tab_changed = false;

    sync_float["fHeight"] = Height;
    sync_float["fWidth"] = Width;
    sync_with_py();
    static bool color_changed = false;
       

static bool isLogin = false, logginIn = false;
static std::string err;
 

if (!isLogin) {
if (ImGui::Begin(oxorany("LOGIN PAGE"),  NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) 
    {
    //background animation start
    
    //=background end animation
           ImGui::Text(oxorany(ICON_FA_USER_SECRET "CONTACT SELLER TO BUY KEY"));
            ImGui::PushItemWidth(-1);
            static char s[150];
             ImGui::InputText(oxorany("##key"), s, sizeof s);
            
            ImGui::PopItemWidth();
            ImGui::PushItemWidth(-1);
            if (ImGui::Button(OBFUSCATE("PASTE"), ImVec2(ImGui::GetWindowContentRegionWidth(), 0))) {
                auto key = getClipboard();
                strncpy(s, key.c_str(), sizeof s);

            }
            ImGui::PopItemWidth();
            
            ImGui::PushItemWidth(-1);
            
            if (ImGui::Button(OBFUSCATE("LOGIN"), ImVec2(ImGui::GetWindowContentRegionWidth(), 0))) {
                err = Login(s);
                if (err == "OK") {
                    isLogin = bValid && g_Auth == g_Token;
                }
            }
            ImGui::PopItemWidth();
            if (!err.empty() && err != std::string(OBFUSCATE("OK"))) {
                ImGui::Text(OBFUSCATE("WRONG KEY!!: %s"), err.c_str());
            }
            ImGui::PopItemWidth();
           }
          

} else {
          
             
        if (menuIconTexture == 0) {
    menuIconTexture = LoadTextureFromArray(Pic_png, sizeof(Pic_png), &iconW, &iconH);
}

// Ukuran ikon yang pas
ImVec2 iconSize = ImVec2(150, 150); // Atau 64x64 jika ingin kecil

// ========== WINDOW ICON ==========
ImGui::SetNextWindowPos(ImVec2(700, 10), ImGuiCond_FirstUseEver);
ImGui::Begin("##icon_button", NULL,
    ImGuiWindowFlags_NoDecoration |
    ImGuiWindowFlags_NoBackground |
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_AlwaysAutoResize
);

// Hanya berlaku di ImageButton saja
ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

if (ImGui::ImageButton((void*)(intptr_t)menuIconTexture, iconSize)) {
    showMenu = !showMenu;
}

ImGui::PopStyleColor(3);
ImGui::PopStyleVar(2);
ImGui::End();
}
ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
ImGui::SetNextWindowSizeConstraints(ImVec2(820, 0), ImVec2(820, FLT_MAX));
if (showMenu && ImGui::Begin(oxorany("             ZENITH SYNDICATE | PREMIUM | 1.003.650007"), &showMenu, window_flags)) {


       
if (ImGui::BeginTabBar("Tab", ImGuiTabBarFlags_FittingPolicyScroll)) {

    // ==== TAB VISUAL ====
    if (ImGui::BeginTabItem("VISUAL")) {
        ImGui::BeginGroupPanel("ESP ", ImVec2(0.0f, 0.0f));
        {
            Toggle(oxorany("ENABLE ESP"), &sync_bool[oxorany("bESP")]);     
            Toggle(oxorany("ESP LINE"), &sync_bool[oxorany("bESP_Line")]);   
            Toggle(oxorany("ESP BOT"), &sync_bool[oxorany("bESP_LineBots")]);
            Toggle(oxorany("ESP BOX"), &sync_bool[oxorany("bESP_Box")]);  
            ImGui::Spacing();
        }
        ImGui::EndGroupPanel();
        
        ImGui::SameLine();

        ImGui::BeginGroup();
        {
            ImGui::BeginGroupPanel("MORE ESP", ImVec2(-1.0f, 0.0f));
            {
                Toggle(oxorany("ESP NAME"), &sync_bool["bESP_Name"]); 
                Toggle(oxorany("ESP HEALTH"), &sync_bool[oxorany("bESP_Health")]);
                Toggle(oxorany("ESP SKELETON"), &sync_bool[oxorany("bESP_Skeleton")]);
                ImGui::Spacing();
           
           }
           ImGui::EndGroupPanel();
           ImGui::BeginGroupPanel("ESP II", ImVec2(-1.0f, 0.0f));
           {
               
           Toggle(oxorany("ESP COUNT"), &isPlayerBotEnabled);
           Toggle(oxorany("ESP DISTANCE"), &sync_bool[oxorany("bESP_Distance")]);          
           ImGui::Spacing();
           }
           ImGui::EndGroupPanel();
           }
           ImGui::EndGroup();
        ImGui::BeginGroupPanel("SETTING ESP:", ImVec2(-1.0f, 0.0f));
        {
            need_sync |= Combo(oxorany("LINE POSITION"), &sync_int[oxorany("iESP_Point")], espPoint, IM_ARRAYSIZE(espPoint));
            need_sync |= SliderFloat(oxorany("SIZE LINE"), &sync_float[oxorany("fESP_LineThickness")], 1.0f, 10.0f, oxorany("%.2f"));
            ImGui::Spacing();
        }
        ImGui::EndGroupPanel();

        ImGui::EndTabItem();
    }
    // ==== TAB AIMBOT ====
    if (ImGui::BeginTabItem("AIMBOT")) {
        ImGui::BeginGroupPanel("AIMBOT MEMORY ", ImVec2(0.0f, 0.0f));
        {
            Toggle(oxorany("ENABLE AIMBOT"), &sync_bool[oxorany("bAIM")]); 
            Toggle(oxorany("CHECK VISIBILITY"), &sync_bool[oxorany("bAIM_CheckVisibility")]);    
            Toggle(oxorany("KNOCKED"), &sync_bool[oxorany("bAIM_IgnoreKnocked")]);
            Toggle(oxorany("BOT"), &sync_bool[oxorany("bAIM_IgnoreBots")]);
            ImGui::Spacing();
        }
        ImGui::EndGroupPanel();
        
        ImGui::SameLine();

        ImGui::BeginGroup();
        {
            ImGui::BeginGroupPanel("BULLET TRACK", ImVec2(-1.0f, 0.0f));
            {
                Toggle(oxorany("ENABLE BULLET"), &sync_bool[oxorany("bBulletTrack")]);
                Toggle(oxorany("IGNORE KNOCK"), &sync_bool[oxorany("bBulletTrack_IgnoreKnocked")]);
                ImGui::Spacing();
           
           }
           ImGui::EndGroupPanel();
           ImGui::BeginGroupPanel("FOV MENU", ImVec2(-1.0f, 0.0f));
           {
               
           Toggle(oxorany("ENABLE FOV"), &sync_bool[oxorany("bAIM_DrawFov")]);
           Toggle(oxorany("HIDE FOV"), &sync_bool[oxorany("bAIM_NoTargetHideFov")]);             
           ImGui::Spacing();
           }
           ImGui::EndGroupPanel();
           }
           ImGui::EndGroup();

        ImGui::BeginGroupPanel("SETTING AIM:", ImVec2(-1.0f, 0.0f));
        {
            need_sync |= Combo(oxorany("TRIGGER"), &sync_int[oxorany("iAIM_Trigger")], aimbotTrigger, IM_ARRAYSIZE(aimbotTrigger));
            need_sync |= Combo(oxorany("TARGET"), &sync_int[oxorany("iAIM_TargetBone")], targetBone, IM_ARRAYSIZE(targetBone)); 
            need_sync |= SliderFloat("STRENGHT", &sync_float[oxorany("fAIM_SnapStrength")], 0.001f, 3.0f, "%.2f"); 
            need_sync |= SliderFloat("FOV", &sync_float[oxorany("fAIM_Fov")], 30.0f, 1500.0f, "%.2f");
            need_sync |= ImGui::SliderFloat("PROBALITY", &sync_float[oxorany("fBulletTrack_Probability")], 30.0f, 100.0f, "%.2f");
        }
        ImGui::EndGroupPanel();

        ImGui::EndTabItem();
    }

    // ==== TAB ChamsBT ====
    if (ImGui::BeginTabItem("CHAMS")) {
        ImGui::BeginGroupPanel("CHAMS SETTING ", ImVec2(0.0f, 0.0f)); 
        {
            need_sync |= ImGui::Combo(oxorany("CHAMS TYPE"), &chamsint, Chams, IM_ARRAYSIZE(Chams));
            Toggle(oxorany("RGB VISIBLE"), &enableRainbow);
            Toggle(oxorany("RGB WALL"), &enableRainbowWall); 
            ImGui::Text(oxorany("IN WALL COLOR"));
            need_sync |= ImGui::ColorEdit4("InWallColor", (float*)&inWallColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip);
            ImGui::Text(oxorany("VISIBLE COLOR"));
            need_sync |= ImGui::ColorEdit4("##VisibleColor", (float*)&visibleColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip);
        }
        ImGui::EndGroupPanel();
        ImGui::EndTabItem();
    }

    // ==== TAB MEMORY ====
    if (ImGui::BeginTabItem("MEMORY")) {
        ImGui::BeginGroupPanel("MEMORY HACK", ImVec2(0.0f, 0.0f));
        {
            Toggle(oxorany("RED WALLHACK"), &sync_bool[oxorany("bXray")]);
            Toggle(oxorany("SPEED HACK"), &sync_bool[oxorany("bSpeed")]);
            Toggle("SKIN HACK", &sync_bool["bSkinHack"]);
            
            ImGui::Spacing();
        }
        ImGui::EndGroupPanel();

        ImGui::SameLine();

        ImGui::BeginGroup();
        {
            ImGui::BeginGroupPanel("AIM HACK", ImVec2(-1.0f, 0.0f));
            {
                Toggle(oxorany("NO RECOIL"), &sync_bool[oxorany("bNoRecoil")]);
                Toggle(oxorany("NO SPREAD"), &sync_bool[oxorany("bNoSpread")]);
                Toggle(oxorany("NO SHAKE"), &sync_bool[oxorany("bSwitch")]);
                Toggle(oxorany("MAGIC BULLET"), &magic);
                if (magic) hexPatches.Magic.Modify(); else hexPatches.Magic.Restore();
                Toggle(oxorany("AIM ASSIST"), &assist);
                if (assist) hexPatches.Assist.Modify(); else hexPatches.Assist.Restore();
                Toggle(oxorany("VISUAL SMOOTH"), &smooth);
                if (smooth) hexPatches.Smooth.Modify(); else hexPatches.Smooth.Restore();
                ImGui::Spacing();
            }
            ImGui::EndGroupPanel();
        }
        ImGui::EndGroup();

        ImGui::BeginGroupPanel("SPEED HACK:", ImVec2(-1.0f, 0.0f));
        {
                need_sync |= SliderFloat("SPEED", &sync_float[oxorany("fSpeed")], 1.0f, 15.0f, "%.2f");
            ImGui::Spacing();
        }
        ImGui::EndGroupPanel();

        ImGui::EndTabItem();
    }



    ImGui::EndTabBar();
}

ImGui::Spacing();
ImGui::Separator();
float t = ImGui::GetTime();
ImVec4 rgbColor = ImVec4(
    0.5f + 0.5f * sin(t * 2.0f),
    0.5f + 0.5f * sin(t * 2.0f + 2.0f),
    0.5f + 0.5f * sin(t * 2.0f + 4.0f),
    1.0f);
ImGui::TextColored(rgbColor, "Created by @EXSTARCOOL | License Zenith syndicate");
ImGui::End();
}



    if (enableRainbow) {
        visibleColor.x = redd / 255.0f;
        visibleColor.y = greenn / 255.0f;
        visibleColor.z = bluee / 255.0f;
    }
    if (enableRainbowWall) {
        inWallColor.x = redd / 255.0f;
        inWallColor.y = greenn / 255.0f;
        inWallColor.z = bluee / 255.0f;
    }
    performRGBChange();
}

static bool setup = false;
DEFINES(EGLBoolean, Draw, EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &Width);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &Height);
    
    if (Width <= 0 || Height <= 0) return _Draw(dpy, surface);
    
    if (!setup) {  
        
 
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    // Set config
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.IniFilename = NULL;

    // Load fonts
    static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
    ImFontConfig icons_config;
    ImFontConfig font_cfg;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 2.5f;
    icons_config.OversampleV = 2.5f;
    io.Fonts->AddFontFromMemoryTTF(
    (void*)roominfofont,
    sizeof(roominfofont),
    28.5f,
    &font_cfg,
    io.Fonts->GetGlyphRangesChineseFull()
);

    io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 45.f, &icons_config, icons_ranges);
    
    ImGui_ImplOpenGL3_Init("#version 300 es");
    ImGui::StyleColorsClassic();
    ImGuiStyle& style = ImGui::GetStyle();
    
    
   
style.Colors[ImGuiCol_Text]                 = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);   // putih terang
style.Colors[ImGuiCol_WindowBg]             = ImVec4(0.05f, 0.05f, 0.05f, 0.94f);   // hampir hitam
style.Colors[ImGuiCol_ChildBg]              = ImVec4(0.10f, 0.10f, 0.10f, 0.90f);   // dark panel
style.Colors[ImGuiCol_Border]               = ImVec4(0.42f, 0.20f, 0.92f, 0.50f);   // ungu soft
style.Colors[ImGuiCol_Button]               = ImVec4(0.20f, 0.20f, 0.20f, 0.60f);   // tidak diubah
style.Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.42f, 0.20f, 0.92f, 0.40f);   // ungu hover
style.Colors[ImGuiCol_ButtonActive]         = ImVec4(0.42f, 0.20f, 0.92f, 0.60f);   // ungu aktif
style.Colors[ImGuiCol_Header]               = ImVec4(0.42f, 0.20f, 0.92f, 0.70f);   // tab ungu aktif
style.Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.25f, 0.12f, 0.55f, 0.80f);   // ungu tua
style.Colors[ImGuiCol_HeaderActive]         = ImVec4(0.42f, 0.20f, 0.92f, 1.00f);   // ungu solid
style.Colors[ImGuiCol_FrameBg]              = ImVec4(0.12f, 0.12f, 0.12f, 0.80f);
style.Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.42f, 0.20f, 0.92f, 0.60f);
style.Colors[ImGuiCol_FrameBgActive]        = ImVec4(0.42f, 0.20f, 0.92f, 0.80f);
style.Colors[ImGuiCol_TitleBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
style.Colors[ImGuiCol_TitleBgActive]        = ImVec4(0.42f, 0.20f, 0.92f, 1.00f);
style.Colors[ImGuiCol_PopupBg]              = ImVec4(0.07f, 0.07f, 0.07f, 0.94f);
style.Colors[ImGuiCol_Tab]                  = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
style.Colors[ImGuiCol_TabHovered]           = ImVec4(0.42f, 0.20f, 0.92f, 0.60f);
style.Colors[ImGuiCol_TabActive]            = ImVec4(0.42f, 0.20f, 0.92f, 0.80f);
style.Colors[ImGuiCol_CheckMark]            = ImVec4(0.42f, 0.20f, 0.92f, 1.00f);
style.Colors[ImGuiCol_SliderGrab]           = ImVec4(0.42f, 0.20f, 0.92f, 0.80f);
style.Colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.42f, 0.20f, 0.92f, 1.00f);
style.Colors[ImGuiCol_Separator]            = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
style.Colors[ImGuiCol_SeparatorHovered]     = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
style.Colors[ImGuiCol_SeparatorActive]      = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
style.Colors[ImGuiCol_ResizeGrip]           = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
style.Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(1.00f, 1.00f, 1.00f, 0.13f);
style.Colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
style.Colors[ImGuiCol_PlotLines]            = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
style.Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);  // opsional ungu juga kalau mau
style.Colors[ImGuiCol_PlotHistogram]        = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
style.Colors[ImGuiCol_TableHeaderBg]        = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
style.Colors[ImGuiCol_TableBorderStrong]    = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
style.Colors[ImGuiCol_TableBorderLight]     = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
style.Colors[ImGuiCol_TableRowBg]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
style.Colors[ImGuiCol_TableRowBgAlt]        = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
style.Colors[ImGuiCol_TextSelectedBg]       = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
style.Colors[ImGuiCol_DragDropTarget]       = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
style.Colors[ImGuiCol_NavWindowingHighlight]= ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
style.Colors[ImGuiCol_NavWindowingDimBg]    = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
style.Colors[ImGuiCol_ModalWindowDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
style.ItemInnerSpacing = ImVec2(8, 6);

style.FrameBorderSize            = 1.0f;





    style.WindowRounding = 7.0f;
    style.ChildRounding = 5.0f;
    style.FrameRounding = 5.0f;
    style.PopupRounding = 5.0f;
    style.ScrollbarRounding = 5.0f;
    style.GrabRounding = 5.0f;
    style.TabRounding = 5.0f;

    style.WindowBorderSize = 2.0f;
    style.ChildBorderSize = 2.0f;
    style.PopupBorderSize = 2.0f;
    style.FrameBorderSize = 2.0f;

    // Spacing and Padding
    style.ItemSpacing = ImVec2(8.0f, 6.0f);
    style.WindowPadding = ImVec2(10.0f, 10.0f);
    style.FramePadding = ImVec2(6.0f, 4.0f);
    style.IndentSpacing = 20.0f;
    style.ScrollbarSize = 1.0f;
    style.ScaleAllSizes(3.0f);
    setup = true;
    }
    
    ImGuiIO& io = ImGui::GetIO();    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(Width, Height);
    NewFrame(); 
    start_entity_reader();
    ClearImGuiWindowTracking();    
    auto draw = GetBackgroundDrawList();
    noMore(draw);
    DrawMenu(io);
    DrawRandomTextLeftBottom();
    EndFrame();
    Render();
    glViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return _Draw(dpy, surface);

}





