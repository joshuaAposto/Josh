#include "include/hpp/imgui.h"
#include <EGL/egl.h>
#include <string>
#include "Dobby/dobby.h"
#include "fake_dlfcn.h"
int Width, Height;
using namespace ImGui;
using namespace std;

const char* INI_PATH = "/data/data/" PACKAGE_NAME "/imgui/.ini";
bool bylogo = true;
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <android/input.h>
#include "curl/json.hpp"
#include "Jni_Stuff.h"
#include <android/keycodes.h>
#include <android/input.h>
#include "Oxorany/oxorany.h"
#include "include/hpp/dlfcn.hpp"
#include "include/hpp/imgui/persistence.h"
#include "include/hpp/imgui/touch.h"
#include "include/sync_with_py.h"
#include "include/sync_entities.h"
ImColor 红色 = ImColor(ImVec4(233/255.f, 55/255.f, 51/255.f, 0.95f));
ImColor 空白 = ImColor(ImVec4(1.0/255.f, 1.0/255.f, 1.0/255.f, 0.0f));
ImColor 白色 = ImColor(ImVec4(255/255.f, 255/255.f, 258/255.f, 0.95f));
ImColor 黑色 = ImColor(ImVec4(0/255.f, 0/255.f, 0/255.f, 0.7f));
#define Yellow ImColor(255, 255, 0, 255)
#define Red ImColor(255, 0, 0, 255)
#define Green ImColor(0, 255, 0, 255)
#include "include/helpers.h"
inline ImVec4 RGBA2ImVec4(int r, int g, int b, int a) {
    float newr = (float)r / 255.0f;
    float newg = (float)g / 255.0f;
    float newb = (float)b / 255.0f;
    float newa = (float)a / 255.0f;
    return ImVec4(newr, newg, newb, newa);
}
#include <string>
#include <sys/system_properties.h>
#include <algorithm> // for std::remove

// Dapatkan model device dari system property
std::string getDeviceName() {
    char model[PROP_VALUE_MAX];
    __system_property_get("ro.product.model", model);
    return std::string(model);
}

// Dapatkan ABI / arsitektur CPU dari system property
std::string getDeviceArchitecture() {
    char abi[PROP_VALUE_MAX];
    __system_property_get("ro.product.cpu.abi", abi);
    std::string arch(abi);

    // Trim newline or extra whitespace just in case
    arch.erase(std::remove(arch.begin(), arch.end(), '\n'), arch.end());
    return arch.empty() ? "Unknown Arch" : arch;
}
static std::string ss1;
static std::string ss2;
static std::string ss3;
static std::string ss4;
static std::string ss5;
static std::string ss6;
static std::string ss7;
static std::string ss8;
static std::string ss9;
static std::string ss10;
static std::string ss11;
static std::string ss12;
static std::string ss13;
static std::string ss14;
static std::string ss15;

bool Zaijan = true;
struct {
    MemoryPatch Feature1;
} var;

void 绘制字体描边(float size,int x, int y, ImVec4 color, const char* str)
{
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x - 0.1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(color), str);
}


bool isVPN()
{
    char command[256] = "";
    memset(command, 0, 256);
    if ((access("/system/bin/ifconfig", F_OK)) != -1){
        sprintf(command, "%s", "/system/bin/ifconfig");
    }else{
        return true;
    }
    FILE* fp = NULL;
    char line[1024] = "";
    fp = popen(command, "r");
    while (fgets(line, 1024, fp) != NULL)
    {
        if (strstr(line, "tun0") != NULL || strstr(line, "ppppp0") != NULL){
            pclose(fp);
            return true;
        }
    }
    pclose(fp);
    return false;
}

void noMore(ImDrawList* draw) {
    if (!sync_bool["bESP"]) return;

    auto entities = get_entities();
    
    int players = 0, bots = 0;
    for (const Entity& e : entities) {
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
            
            auto limbs_r_foot = ToImVec2(e.bones.at("limbs_r_foot"));
            auto limbs_r_toe0 = ToImVec2(e.bones.at("limbs_r_toe0"));
            
            auto limbs_l_upperarm = ToImVec2(e.bones.at("limbs_l_upperarm"));
            auto limbs_l_forearm = ToImVec2(e.bones.at("limbs_l_forearm"));
            auto limbs_l_hand = ToImVec2(e.bones.at("limbs_l_hand"));
            
            auto limbs_l_thigh = ToImVec2(e.bones.at("limbs_l_thigh"));
            auto limbs_l_calf = ToImVec2(e.bones.at("limbs_l_calf"));
            
            auto limbs_l_foot = ToImVec2(e.bones.at("limbs_l_foot"));
            auto limbs_l_toe0 = ToImVec2(e.bones.at("limbs_l_toe0"));
            
            ImVec2 chest((limbs_r_upperarm.x + limbs_l_upperarm.x) / 2, (limbs_r_upperarm.y + limbs_l_upperarm.y) / 2);
            
            auto linecolor = e.is_visible ? colors["cESP_Skeleton"] : colors["cESP_SkeletonHidden"];
            auto thickness = sync_float["fESP_LineThickness"];
			
draw->AddLine(head, chest, linecolor, thickness);
draw->AddLine(chest, spine, linecolor, thickness);

// Tangan
draw->AddLine(chest, limbs_r_upperarm, linecolor, thickness);
draw->AddLine(chest, limbs_l_upperarm, linecolor, thickness);
draw->AddLine(limbs_r_upperarm, limbs_r_forearm, linecolor, thickness);
draw->AddLine(limbs_l_upperarm, limbs_l_forearm, linecolor, thickness);
draw->AddLine(limbs_r_forearm, limbs_r_hand, linecolor, thickness);
draw->AddLine(limbs_l_forearm, limbs_l_hand, linecolor, thickness);

// Kaki
draw->AddLine(spine, limbs_r_thigh, linecolor, thickness);
draw->AddLine(spine, limbs_l_thigh, linecolor, thickness);

draw->AddLine(limbs_r_thigh, limbs_r_calf, linecolor, thickness);
draw->AddLine(limbs_l_thigh, limbs_l_calf, linecolor, thickness);

draw->AddLine(limbs_r_calf, limbs_r_foot, linecolor, thickness);
draw->AddLine(limbs_l_calf, limbs_l_foot, linecolor, thickness);

draw->AddLine(limbs_r_foot, limbs_r_toe0, linecolor, thickness);
draw->AddLine(limbs_l_foot, limbs_l_toe0, linecolor, thickness);
        }


        ImVec2 To(ToImVec2(e.bones.at("head")));

        
        if (e.is_bot) {
            if (sync_bool["bESP_LineBots"]) draw->AddLine(From, To, e.is_visible ? colors["cESP_LineBots"] : colors["cESP_LineBotsHidden"], sync_float["fESP_LineThickness"]);
        } else {
            if (sync_bool["bESP_Line"]) draw->AddLine(From, To, e.is_visible ? colors["cESP_Line"] : colors["cESP_LineHidden"], sync_float["fESP_LineThickness"]);
        }
/*if (sync_bool["bAIM_SnapLine"]) {
    
// if (sync_bool["bAIM_SnapLine"] && e.is_snap) draw->AddLine(ImVec2(Width / 2, Height / 2), To, e.is_visible ? colors["cAIM_Line"] : colors["cAIM_LineHidden"], sync_float["fESP_LineThickness"]);

ImVec2 screenCenter = ImVec2(ImGui::GetIO().DisplaySize.x / 2.f, ImGui::GetIO().DisplaySize.y / 2.f);
float aimFov = sync_float["fAimbotFov"]; // Misal: 50.0f
float maxLineDistance = aimFov; // Radius batas FOV (di layar)

// Hitung jarak ke crosshair (layar)
float dx = To.x - screenCenter.x;
float dy = To.y - screenCenter.y;
float distanceToCrosshair = sqrtf(dx * dx + dy * dy);

// Tampilkan garis hanya jika dalam FOV dan terlihat
if (e.is_visible && distanceToCrosshair <= maxLineDistance) {
    ImU32 aimColor = IM_COL32(255, 255, 0, 255); // Kuning terang
    float thickness = sync_float["fESP_LineThickness"];
//if (e.id == closestTarget.id && e.is_visible) {
    //draw->AddLine(screenCenter, To, IM_COL32(255, 50, 50, 255), sync_float["fESP_LineThickness"]);
//}

    draw->AddLine(screenCenter, To, aimColor, thickness);
}

}*/
ImVec2 head = ToImVec2(e.bones.at("head"));
ImVec2 foot_l = ToImVec2(e.bones.at("limbs_l_toe0"));
    ImVec2 foot_r = ToImVec2(e.bones.at("limbs_r_toe0"));
        ImVec2 feet = (foot_l.y > foot_r.y) ? foot_l : foot_r;
        float height = feet.y - head.y;
        float width = height * 0.5f;
        float distance = 200.0f / height;


        if (sync_bool["bESP_Name"] && !e.bones.empty()) {
    ImVec2 head = ToImVec2(e.bones.at("head"));
    ImVec2 foot_l = ToImVec2(e.bones.at("limbs_l_toe0"));
    ImVec2 foot_r = ToImVec2(e.bones.at("limbs_r_toe0"));
    ImVec2 feet = (foot_l.y > foot_r.y) ? foot_l : foot_r;

    float height = feet.y - head.y;
    float width = height / 2.5f;

    // Posisi box (pakai full ukuran box ESP)
    float boxX = head.x - (width / 2);
    float boxY = head.y;

    // Nama
    std::string name = e.is_bot ? "BOT" : "PLAYER";
    if (e.is_knocked) name += " (KNOCKED)";

    ImVec2 nameSize = ImGui::CalcTextSize(name.c_str());

    // Posisi teks: tengah horizontal di atas box
    ImVec2 textPos = ImVec2(boxX + (width / 2) - (nameSize.x / 2), boxY - nameSize.y - 4.0f);

    draw->AddText(
        textPos,
        e.is_visible ? ImColor(0, 255, 0) : ImColor(150, 150, 150),
        name.c_str()
    );
}

        // Distance ESP
 if (sync_bool["bESP_Distance"] && !e.bones.empty()) {
    ImVec2 head = ToImVec2(e.bones.at("head"));
    ImVec2 foot_l = ToImVec2(e.bones.at("limbs_l_toe0"));
    ImVec2 foot_r = ToImVec2(e.bones.at("limbs_r_toe0"));
    ImVec2 feet = (foot_l.y > foot_r.y) ? foot_l : foot_r;

    float height = feet.y - head.y;
    float width = height / 2.5f;

    // Hitung posisi box
    float boxX = head.x - (width / 2);
    float boxBottomY = head.y + height;

    // Format teks jarak
    char distText[16];
    snprintf(distText, sizeof(distText), "%.0fcm", distance);

    ImVec2 distSize = ImGui::CalcTextSize(distText);

    // Posisi teks: tepat di tengah bawah box
    ImVec2 textPos;
    textPos.x = boxX + (width / 2.0f) - (distSize.x / 2.0f);
    textPos.y = boxBottomY + 3.0f;

    // Gambar teks
    draw->AddText(
        textPos,
        ImColor(0, 255, 0), // Warna hijau tetap
        distText
    );
}


if (sync_bool["bESP_Health"] && !e.bones.empty()) {
    ImVec2 head = ToImVec2(e.bones.at("head"));
    ImVec2 foot_l = ToImVec2(e.bones.at("limbs_l_toe0"));
    ImVec2 foot_r = ToImVec2(e.bones.at("limbs_r_toe0"));
    ImVec2 feet = (foot_l.y > foot_r.y) ? foot_l : foot_r;

    float height = feet.y - head.y;
    float width = height / 2.5f;

    // Posisi box
    float box_x = head.x - (width / 2);
    float box_y = head.y;

    // Ukuran Health Bar
    float bar_width = 4.0f;
    float bar_height = height;

    // Posisi Health Bar di kiri box
    float bar_x = box_x - 6.0f;
    float bar_y = box_y;

    // Hitung health
    float max_health = 125.0f;
    float health_percent = std::clamp(e.is_health / max_health, 0.0f, 1.0f);
    float filled_height = bar_height * health_percent;
    float filled_y = bar_y + (bar_height - filled_height); // dari bawah ke atas

    // Warna dinamis (merah → hijau)
    ImColor healthColor = ImColor::HSV(health_percent * 0.33f, 1.0f, 1.0f);

    // Background bar (kosong)
    draw->AddRectFilled(
        ImVec2(bar_x, bar_y),
        ImVec2(bar_x + bar_width, bar_y + bar_height),
        IM_COL32(0, 0, 0, 160),
        2.0f
    );

    // Isi health
    draw->AddRectFilled(
        ImVec2(bar_x, filled_y),
        ImVec2(bar_x + bar_width, bar_y + bar_height),
        healthColor,
        2.0f
    );

    // Outline
    draw->AddRect(
        ImVec2(bar_x, bar_y),
        ImVec2(bar_x + bar_width, bar_y + bar_height),
        IM_COL32(0, 0, 0, 200),
        1.5f
    );
}
/*
if (sync_bool["bESP_Health"] && !e.bones.empty()) {
    ImVec2 head = ToImVec2(e.bones.at("head"));

    float max_armor = 125.0f;
    float armor_percent = std::min(e.is_armor / max_armor, 1.0f);
    float bar_width = 70.0f;
    float filled_width = bar_width * armor_percent;

    float startX = head.x - bar_width / 2;
    float topY = head.y - 4;
    float bottomY = head.y + 2;

    // Isi bar
    draw->AddRectFilled({startX, bottomY}, {startX + filled_width, topY}, IM_COL32(0, 170, 255, 255), 0);
    // Border
    draw->AddRect({startX, bottomY}, {startX + bar_width, topY}, IM_COL32(0, 0, 0, 255), 0, 0);
}
*/
	
//ImVec2 To(ToImVec2(e.bones.at("head")));  
if (sync_bool["bESP_Box"] && !e.bones.empty() && e.bones.count("head") && e.bones.count("limbs_l_toe0") && e.bones.count("limbs_r_toe0")) {
    ImVec2 head   = ToImVec2(e.bones.at("head"));
    ImVec2 foot_l = ToImVec2(e.bones.at("limbs_l_toe0"));
    ImVec2 foot_r = ToImVec2(e.bones.at("limbs_r_toe0"));
    ImVec2 feet   = (foot_l.y > foot_r.y) ? foot_l : foot_r;

    // Skala agar lebih proporsional (total tinggi akan 1.15x dari head-to-foot asli)
    float scale = 1.26f;

    float rawHeight = (feet.y - head.y);
    float height = rawHeight * scale;
    float width  = height * 0.50f;

    // Offset dari kepala (biar head tidak tertarik ke bawah)
    ImVec2 topLeft     = { head.x - (width / 2.0f), head.y - (rawHeight * 0.30f) };
    ImVec2 bottomRight = { topLeft.x + width, topLeft.y + height };

    float thickness = sync_float["fESP_LineThickness"];
    ImU32 lineColor = e.is_visible ? colors["cESP_Box"] : colors["cESP_BoxHidden"];

    // Ukuran sudut
    float lineW = width  * 0.25f;
    float lineH = height * 0.18f;

    // ──────────────── Gambar Sudut Kotak ────────────────
    // Atas Kiri
    draw->AddLine(topLeft, { topLeft.x + lineW, topLeft.y }, lineColor, thickness);
    draw->AddLine(topLeft, { topLeft.x, topLeft.y + lineH }, lineColor, thickness);

    // Atas Kanan
    draw->AddLine({ bottomRight.x - lineW, topLeft.y }, { bottomRight.x, topLeft.y }, lineColor, thickness);
    draw->AddLine({ bottomRight.x, topLeft.y }, { bottomRight.x, topLeft.y + lineH }, lineColor, thickness);

    // Bawah Kiri
    draw->AddLine({ topLeft.x, bottomRight.y - lineH }, { topLeft.x, bottomRight.y }, lineColor, thickness);
    draw->AddLine({ topLeft.x, bottomRight.y }, { topLeft.x + lineW, bottomRight.y }, lineColor, thickness);

    // Bawah Kanan
    draw->AddLine({ bottomRight.x - lineW, bottomRight.y }, { bottomRight.x, bottomRight.y }, lineColor, thickness);
    draw->AddLine({ bottomRight.x, bottomRight.y - lineH }, { bottomRight.x, bottomRight.y }, lineColor, thickness);
}

if (sync_bool["bESP_DangerCircle"] && e.is_visible < 15.0f) {
    draw->AddCircle(To, 30.0f, IM_COL32(255, 0, 0, 180), 32, 2.0f);
    draw->AddText(ImVec2(To.x - 20, To.y + 25), IM_COL32(255, 0, 0, 255), "DANGER!");
}


}
    
std::string yan = "";
auto textSize = ImGui::CalcTextSize2(yan.c_str(), 0, ((float) Width / 15.5f));
draw->AddText(NULL, ((float) Width / 10.0f), {((float) Width / 2.05f) - (textSize.x / 2), 550}, 红色, yan.c_str());

if (players + bots == 0) {
    yan += "Zora";
    draw->AddRectFilled({((Width / 2)) - 70, 70}, {((Width / 2)) + 70, 110}, 0x90000000, 5, 0);
    auto textSize = ImGui::CalcTextSize2(yan.c_str(), 0, 40);  // Ukuran teks diperbesar
    draw->AddText(NULL, 40, {((Width / 2)) - (textSize.x / 2), 75}, ImColor(0, 255, 0), yan.c_str());
} else {
    yan += "Player[";
    yan += std::to_string((int)players);
    yan += "]";
    yan += " | ";
    yan += "[";
    yan += std::to_string((int)bots);
    yan += "]Bots";

    draw->AddRectFilled({((Width / 2)) - 120, 70}, {((Width / 2)) + 120, 110}, 0x90000000, 5, 0);
    auto textSize = ImGui::CalcTextSize2(yan.c_str(), 0, 40);  // Ukuran teks diperbesar
    draw->AddText(NULL, 40, {((Width / 2)) - (textSize.x / 2), 75}, ImColor(255, 255, 255), yan.c_str());
}

        if (!sync_bool["bAIM_NoTargetHideFov"] || (players || bots)) {
               
        if (sync_bool["bAIM_DrawFov"]) draw->AddCircle(ImVec2(Width / 2, Height / 2), sync_float["fAIM_Fov"], WHITE, 0, sync_float["fESP_LineThickness"]);
        if (sync_bool["ADSKCM"]) draw->AddCircle(ImVec2(Width / 2, Height / 2), sync_float["fAIM_Fov"], BLACK, 0, sync_float["fESP_LineThickness"]);
     //   if (players || bots) { draw->drawText(("P:"+to_string(players)+" B:"+to_string(bots)).c_str(), 70, ImVec2(Width / 2, 120), CYAN); }
      //  else { draw->drawText("SAFE", 70, ImVec2(Width / 2, 120), WHITE); }
    } else { //draw->drawText("SAFE", 70, ImVec2(Width / 2, 120), WHITE); 
    }
}
void DrawMenu(ImGuiIO &io) {
    //SetNextWindowSizeConstraints(ImVec2(800, 400), ImVec2(FLT_MAX, FLT_MAX));
 //   SetNextWindowSize(ImVec2(1000, 700), ImGuiCond_FirstUseEver);
    bool need_sync = false, tab_changed = false;

    sync_float["fHeight"] = Height;
    sync_float["fWidth"] = Width;
	
    // sync_with_py();
   

if (!Zaijan) {
ImGui::Text(OBFUSCATE("KOMTOL "));
    
}

if (!g_Token.empty() && !g_Auth.empty() && g_Token == g_Auth) {
bool Zaijan = false;
}
//LOGIN PAGE l SE7ENSINS
/*if (!isLogin) {
if (ImGui::Begin(oxorany("LOGIN PAGE"),  NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) 
	{
	//background animation start
	
	//=background end animation
           ImGui::Text(oxorany("  BUY KEY l @AnakJahatWillmods "));
            ImGui::PushItemWidth(-1);
            static char s[150];
            ImGui::InputText("##key", s, sizeof s);
            
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
			ImGui::Text("Device Info:");
                ImGui::Text("Device Model: %s", getDeviceName().c_str());
                ImGui::Text("CPU Architecture: %s", getDeviceArchitecture().c_str());
           }
          
            } else {*/
				
            
	static std::string current_tab = "";
static bool HideWindow = true;
ImGui::Begin("KONTOL", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground); {
                const ImVec2 pos = ImGui::GetWindowPos();
                ImDrawList* draw = ImGui::GetWindowDrawList();               
                ImGui::SetCursorPos({ 50, 50});
                if(ImGui::Button(" KONTOL", ImVec2(50,50))){HideWindow = !HideWindow;}
            //    DrawImage(pos.x + 25, pos.y + 25, 80, 80, Logo.textureId);   
                }
		static bool showMenu = true; // Control visibility toggle
static bool isLogin = false, logginIn = false;
static std::string err;
bool bFullChecked = false;
static bool MenuShow;
//static std::string err;
static char s[64];
static const char *keyexpired = "Hubungi Seller Untuk Membeli Key";
int selectedFeature = 0;

ImGui::Begin("Dolphin Mod", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);{
	
	if (HideWindow) {
ImGui::SetNextWindowSize(ImVec2(800, 700), ImGuiCond_Always); // Optional: fix window size
//lebar tinggi
ImGui::Begin( "Menu",&HideWindow,ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
ImGui::Text("ZORA | PREMIUM MODZS                                                 ");
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {15, 15});
            if (ImGui::BeginChild("##左侧菜单标题", ImVec2(142, -1), false, ImGuiWindowFlags_NoScrollbar));
{

           // ImGui::Image(top.textureId, ImVec2(140, 140));//图形大小 宽 长
            ImGui::SetCursorPos({0, 143});//和底下的距离↓
           // ImGui::Separator();
            ImGui::Spacing();
            static int Freedom = 1;
            if (ImGui::Button("Visual", ImVec2(145, 45))){
                Freedom = 1;
                }
            if (ImGui::Button("Aimbot", ImVec2(145, 45))){
                Freedom = 2;
                }
            if (ImGui::Button("Silent", ImVec2(145, 45))){
                Freedom = 3;
                }
            if (ImGui::Button("Memory", ImVec2(145, 45))){
                Freedom = 4;
                }
            if (ImGui::Button("Hide Window", ImVec2(145, 45))){
                HideWindow = false;
                }
            
       //     ImGui::Checkbox("Active  ", &Config.ESPMenu.Activity);
      //      ImGui::Checkbox("Antiban", &Config.Bypass);
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Text("\n%.3f MS \nfsp(%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

                ImGui::EndChild();
                ImGui::PopStyleVar(1);
                ImGui::SameLine();
          //      SeparatorEx(ImGuiSeparatorFlags_Vertical);
                ImGui::SameLine();
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {15, 15});
                ImGui::SameLine();
                if (ImGui::BeginChild("##begin2", ImVec2(-1, -1), false)){
                time_t tt_;
                struct tm *tm_ ;
                {
                time(&tt_);
                tm_ = localtime(&tt_);
                ImGui::Text("Hours                               %02d:%02d:%02d\r ", tm_->tm_hour, tm_->tm_min, tm_->tm_sec);
                if (tm_->tm_hour == 23 && tm_->tm_min == 59 && tm_->tm_sec == 59)
                {
                exit(0);
                }
                if (tm_->tm_hour == 23 && tm_->tm_min == 50 && tm_->tm_sec == 59)
                {
                ss15 = "";
                }
                }          
                ImGui::Separator();
                
                if (ImGui::BeginChild("##Begin", ImVec2(0, 0), false, ImGuiTabBarFlags_FittingPolicyScroll))
{
                if (Freedom == 1) {
                ImGui::Text("Extra Visual");
                ImGui::Spacing();
                if (ImGui::BeginTable("split", 4));{
                ImGui::TableNextColumn();
                need_sync |= Checkbox("esp line", &sync_bool["bESP_Line"]);
                ImGui::TableNextColumn();
                need_sync |= Checkbox("esp line for bots", &sync_bool["bESP_LineBots"]);
                ImGui::TableNextColumn();
                need_sync |= Checkbox("esp skeleton", &sync_bool["bESP_Skeleton"]);
                ImGui::TableNextColumn();
                need_sync |= Checkbox("esp Box", &sync_bool["bESP_Box"]); 		
                ImGui::NextColumn();
                need_sync |= Checkbox("esp Distance", &sync_bool["bESP_Distance"]);      
                ImGui::TableNextColumn();     
                need_sync |= Checkbox("esp Healt", &sync_bool["bESP_Health"]);	
                ImGui::TableNextColumn();			
	            need_sync |= Checkbox("esp name", &sync_bool["bESP_Name"]);
	            ImGui::TableNextColumn();
	            need_sync |= Checkbox("esp Team id", &sync_bool["bESP_TeamID"]);
	         	ImGui::TableNextColumn();
                //menu
                ImGui::EndTable();	
                }
                ImGui::Separator();
                static bool color_changed = false;
        ImGui::Text(oxorany(" Color Line Bots"));
        SameLine();
                    color_changed |= ImGui::ColorEdit4("##line bots color", &colors["cESP_LineBots"].Value.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip);
                    SameLine();
                    color_changed |= ImGui::ColorEdit4("##line bots hidden color", &colors["cESP_LineBotsHidden"].Value.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip);
          ImGui::Text(oxorany(" Color Line Player"));
        SameLine();
                    color_changed |= ImGui::ColorEdit4("##line color", &colors["cESP_Line"].Value.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip);
                    SameLine();
                    color_changed |= ImGui::ColorEdit4("##line hidden color", &colors["cESP_LineHidden"].Value.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip);
   SameLine();
        
        ImGui::Text(oxorany(" Color Skeleton"));
        SameLine();
                    color_changed |= ImGui::ColorEdit4("##skeleton color", &colors["cESP_Skeleton"].Value.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip);
                    SameLine();
                    color_changed |= ImGui::ColorEdit4("##skeleton hidden color", &colors["cESP_SkeletonHidden"].Value.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip);
              ImGui::Separator();
              ImGui::Text("Esp Line Thickness Settings");                
                need_sync |= SliderFloat("line thickness", &sync_float["fESP_LineThickness"], 1.0f, 10.0f, "%.2f");
                ImGui::Text("Esp Starting Point Settings");                
                need_sync |= Combo("starting point", &sync_int["iESP_Point"], espPoint, IM_ARRAYSIZE(espPoint));
                ImGui::Separator();
                //menu
                }
                if (Freedom == 2) {
if (ImGui::BeginTable("split", 4));{
     static bool color_changed = false;
                    //  ImGui::BeginGroupPanel("AIMBOT FEATURE", ImVec2(-1, 0));			          	
					//	ImGui::Spacing();
					ImGui::Separator();
			    need_sync |= Checkbox("enable", &sync_bool["bAIM"]);
			    ImGui::TableNextColumn();
                need_sync |= Checkbox("check visibility", &sync_bool["bAIM_CheckVisibility"]);
                ImGui::TableNextColumn();
                need_sync |= Checkbox("ignore knocked", &sync_bool["bAIM_IgnoreKnocked"]);
                ImGui::TableNextColumn();
                need_sync |= Checkbox("ignore bots", &sync_bool["bAIM_IgnoreBots"]);
                ImGui::TableNextColumn();
                need_sync |= Checkbox("draw fov", &sync_bool["bAIM_DrawFov"]);
                ImGui::TableNextColumn();
                need_sync |= Checkbox("hide fov if there's no target", &sync_bool["bAIM_NoTargetHideFov"]);
						//ImGui::EndGroupPanel();	
    
    ImGui::EndTable();
       }
       ImGui::Separator();
need_sync |= SliderFloat("snap strength", &sync_float["fAIM_SnapStrength"], 0.001f, 4.00f, "%.3f"); 
need_sync |= SliderFloat("fov", &sync_float["fAIM_Fov"], 30.0f, 1500.0f, "%.2f");
need_sync |= Combo("trigger", &sync_int["iAIM_Trigger"], aimbotTrigger, IM_ARRAYSIZE(aimbotTrigger));
need_sync |= Combo("target bone", &sync_int["iAIM_TargetBone"], targetBone, IM_ARRAYSIZE(targetBone));
}

if (Freedom == 3) {
if (ImGui::BeginTable("split", 4));{
need_sync |= Checkbox("Silent Aim", &sync_bool["bBulletTrack"]);
need_sync |= SliderFloat("Probalbility Silent", &sync_float["fBulletTrack_Probability"], 70.0f, 1500.0f, "%.3f");
need_sync |= Checkbox("draw fov", &sync_bool["ADSKCM"]);
need_sync |= Checkbox("ignore bots", &sync_bool["bBulletTrack_IgnoreBots"]);
need_sync |= Checkbox("ignore Knock", &sync_bool["bBulletTrack_IgnoreKnocked"]);
ImGui::EndTable();
       }}

if (Freedom == 4) {
 if (ImGui::BeginTable("split", 4));{
need_sync |= Checkbox("SkinHack", &sync_bool["bSkinHack"]);
            need_sync |= Checkbox("Magick Bullet", &sync_bool["bFeature1"]);
            if (sync_bool["bFeature1"] && !last_sync_bool["bFeature1"]) {
                var.Feature1.Modify();
            } else if (!sync_bool["bFeature1"] && last_sync_bool["bFeature1"]) {
                var.Feature1.Restore();   
            }
		    need_sync |= Checkbox("NoRecoil", &sync_bool["bNoRecoil"]);
		  //  ImGui::NextColumn();
		   // need_sync |= Checkbox("NoRecoil v2", &sync_bool["bNoRecoil"]);
			need_sync |= Checkbox("NoSpread", &sync_bool["bNoSpread"]);
            need_sync |= Checkbox("wall hack", &sync_bool["bXray"]);
ImGui::EndTable();
       }}
                }

                
                ImGui::Spacing();
                ImGui::EndTabItem();	 
                }
                }
                
	}}
      End();
}
int screenWidth = -1, screenHeight = -1;
static bool setup = false;
int32_t (*orig_ANativeWindow_getWidth)(ANativeWindow *window);
int32_t _ANativeWindow_getWidth(ANativeWindow *window)
{
screenWidth = orig_ANativeWindow_getWidth(window);
return orig_ANativeWindow_getWidth(window);
}

int32_t (*orig_ANativeWindow_getHeight)(ANativeWindow *window);
int32_t _ANativeWindow_getHeight(ANativeWindow *window)
{
screenHeight = orig_ANativeWindow_getHeight(window);
return orig_ANativeWindow_getHeight(window);
}

ImVec2 windowPos = ImVec2(50, 50);
ImVec2 touchStart;
bool isDragging = false;
bool windowOpen = true;
ImVec2 SmoothMove(ImVec2 start, ImVec2 end, float speed) {
return ImVec2(start.x + (end.x - start.x) * speed, start.y + (end.y - start.y) * speed);
}
void *protectFunc(void *addr) {
size_t pageSize = sysconf(_SC_PAGESIZE);
void *alignedAddr = (void *)((uintptr_t)addr & ~(pageSize - 1));
mprotect(alignedAddr, pageSize, PROT_READ | PROT_WRITE | PROT_EXEC);
return addr;
}
int32_t (*orig_AInputQueue_getEvent)(AInputQueue *queue, AInputEvent **outEvent);
int32_t hooked_AInputQueue_getEvent(AInputQueue *queue, AInputEvent **outEvent) {
    int32_t res = orig_AInputQueue_getEvent(queue, outEvent);
    if (res >= 0 && setup) {
        int eventType = AInputEvent_getType(*outEvent);

        ImGuiIO& io = ImGui::GetIO();

        if (eventType == AINPUT_EVENT_TYPE_MOTION) { // Handle touchscreen input
            int action = AMotionEvent_getAction(*outEvent) & AMOTION_EVENT_ACTION_MASK;
            float x = AMotionEvent_getX(*outEvent, 0);
            float y = AMotionEvent_getY(*outEvent, 0);

            float normX = (x / (float)Width) * Width;
            float normY = (y / (float)Height) * Height;

            io.MousePos = ImVec2(normX, normY);

            switch (action) {
                case AMOTION_EVENT_ACTION_DOWN:
                    io.MouseDown[0] = true;
                    isDragging = true;
                    touchStart = ImVec2(normX - windowPos.x, normY - windowPos.y);
                    break;

                case AMOTION_EVENT_ACTION_MOVE:
                    if (isDragging) {
                        ImVec2 targetPos = ImVec2(normX - touchStart.x, normY - touchStart.y);
                        windowPos = SmoothMove(windowPos, targetPos, 0.2f);
                    }
                    break;

                case AMOTION_EVENT_ACTION_UP:
                    io.MouseDown[0] = false;
                    isDragging = false;
                    break;
            }
            return 1;
        } 
    }
    return res;
}

DEFINES(EGLBoolean, Draw, EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &Width);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &Height);
    
    if (Width <= 0 || Height <= 0) return _Draw(dpy, surface);
    
    if (!setup) {
        setup = true;
        CreateContext();
                
        ImGui_ImplOpenGL3_Init("#version 300 es");
        
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
        
        switch_theme(current_theme);

        load_features();
        load_imgui_style();
        load_colors();

        io.IniFilename = features["bImguiAutoSave"] ? INI_PATH : nullptr;
        ImFontConfig font_cfg;
        font_cfg.SizePixels = features_float["fFontScale"];
        io.Fonts->AddFontDefault(&font_cfg);
    }
    
    ImGuiIO& io = ImGui::GetIO();
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(Width, Height);
    NewFrame();
    
    ClearImGuiWindowTracking();
    
    auto draw = GetBackgroundDrawList();
   // draw->AddLine(ImVec2(2400, 1080), ImVec2(0,0), WHITE, 0.1);
    noMore(draw);

    DrawMenu(io);

    EndFrame();
    Render();
    ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
    
    return _Draw(dpy, surface);
}

void __IMGUI__() {
    start_entity_reader();
	
void *libAndroid = dlopen_ex(("libandroid.so"), 4);
if (libAndroid) {
void *symEvent1 = dlsym_ex(libAndroid, ("ANativeWindow_getWidth"));

DobbyHook(symEvent1, (void *)_ANativeWindow_getWidth, (void **)&orig_ANativeWindow_getWidth);

void *symEvent2 = dlsym_ex(libAndroid, ("ANativeWindow_getHeight"));

DobbyHook(symEvent2, (void *)_ANativeWindow_getHeight, (void **)&orig_ANativeWindow_getHeight);

void *symEvent = dlsym_ex(libAndroid, ("AInputQueue_getEvent"));
protectFunc(symEvent);
DobbyHook(symEvent, (void *)hooked_AInputQueue_getEvent, (void **)&orig_AInputQueue_getEvent);
}
    HOOKIT("libEGL.so", "eglSwapBuffers", Draw);
    
    LOGI("RETURNING FROM __IMGUI__");
}
