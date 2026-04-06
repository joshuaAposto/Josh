#include "include/hpp/imgui.h"
#include <EGL/egl.h>
#include <string>

int Width, Height;
using namespace ImGui;
using namespace std;

const char* INI_PATH = "/data/data/" PACKAGE_NAME "/imgui/.ini";

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

#include "include/helpers.h"

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


bool Zaijan = true;
struct {
    MemoryPatch Feature1;
} var;

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
            
            auto limbs_l_upperarm = ToImVec2(e.bones.at("limbs_l_upperarm"));
            auto limbs_l_forearm = ToImVec2(e.bones.at("limbs_l_forearm"));
            auto limbs_l_hand = ToImVec2(e.bones.at("limbs_l_hand"));
            
            auto limbs_l_thigh = ToImVec2(e.bones.at("limbs_l_thigh"));
            auto limbs_l_calf = ToImVec2(e.bones.at("limbs_l_calf"));
            
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
        }

        ImVec2 To(ToImVec2(e.bones.at("head")));

        
        if (e.is_bot) {
            if (sync_bool["bESP_LineBots"]) draw->AddLine(From, To, e.is_visible ? colors["cESP_LineBots"] : colors["cESP_LineBotsHidden"], sync_float["fESP_LineThickness"]);
        } else {
            if (sync_bool["bESP_Line"]) draw->AddLine(From, To, e.is_visible ? colors["cESP_Line"] : colors["cESP_LineHidden"], sync_float["fESP_LineThickness"]);
        }
if (sync_bool["bAIM_SnapLine"]) {
    
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

}

if ((sync_bool["bESP_TeamID"] || sync_bool["bESP_Name"] || sync_bool["bESP_Distance"]) && !e.bones.empty()) {
    ImVec2 head = ToImVec2(e.bones.at("head"));
    ImVec2 foot_l = ToImVec2(e.bones.at("limbs_l_calf"));
    ImVec2 foot_r = ToImVec2(e.bones.at("limbs_r_calf"));
    ImVec2 feet = (foot_l.y > foot_r.y) ? foot_l : foot_r;

    float height = feet.y - head.y;
    float width = height * 0.6f;
    ImVec2 boxMin(head.x - width / 2, head.y);
    ImVec2 boxMax(head.x + width / 2, feet.y);
    ImColor fillColor = e.is_visible ? ImColor(0, 255, 0, 90) : ImColor(0, 255, 0, 40);
    draw->AddRectFilled(boxMin, boxMax, fillColor);

    float distance = 200.0f / height;

    // Ganti nama jadi "KNOCK" jika pemain knock
    const char* tag = e.is_knocked ? "KNOCK" : (e.is_bot ? "BOT" : "PLAYER");

    std::string teamText = std::to_string(e.team_id);
    std::string distanceText = std::to_string((int)distance) + "m";

    ImVec2 teamSize = ImGui::CalcTextSize(teamText.c_str());
    ImVec2 tagSize = ImGui::CalcTextSize(tag);
    ImVec2 distSize = ImGui::CalcTextSize(distanceText.c_str());

    float padding = 6.0f;
    float spacing = 10.0f;
    float totalWidth = padding;
    if (sync_bool["bESP_TeamID"])    totalWidth += teamSize.x + spacing;
    if (sync_bool["bESP_Name"])      totalWidth += tagSize.x + spacing;
    if (sync_bool["bESP_Distance"])  totalWidth += distSize.x;
    totalWidth += padding;

    float boxHeight = std::max({ teamSize.y, tagSize.y, distSize.y }) + 4;

    ImVec2 boxPos = ImVec2(head.x - totalWidth / 2, head.y - boxHeight - 10);
    ImVec2 boxEnd = ImVec2(boxPos.x + totalWidth, boxPos.y + boxHeight);

    draw->AddRectFilled(boxPos, boxEnd, IM_COL32(0, 0, 0, 180), 3.0f);
    draw->AddRect(boxPos, boxEnd, IM_COL32(255, 255, 255, 40), 3.0f);

    float cursorX = boxPos.x + padding;
    if (sync_bool["bESP_TeamID"]) {
        draw->AddText(ImVec2(cursorX, boxPos.y + 2), IM_COL32(255, 255, 255, 255), teamText.c_str());
        cursorX += teamSize.x + spacing;
    }
    if (sync_bool["bESP_Name"]) {
        ImU32 nameColor = e.is_knocked ? IM_COL32(255, 0, 0, 255) : IM_COL32(255, 255, 255, 255);
        draw->AddText(ImVec2(cursorX, boxPos.y + 2), nameColor, tag);
        cursorX += tagSize.x + spacing;
    }
    if (sync_bool["bESP_Distance"]) {
        draw->AddText(ImVec2(cursorX, boxPos.y + 2), IM_COL32(0, 255, 0, 255), distanceText.c_str());
    }
}

 
    if (sync_bool["bESP_Health"] && !e.bones.empty()) {
        ImVec2 head = ToImVec2(e.bones.at("head"));
    ImVec2 foot_l = ToImVec2(e.bones.at("limbs_l_calf"));
    ImVec2 foot_r = ToImVec2(e.bones.at("limbs_r_calf"));
    ImVec2 feet = (foot_l.y > foot_r.y) ? foot_l : foot_r;

    float height = feet.y - head.y;
    float width = height * 0.6f;
    ImVec2 boxMin = ImVec2(head.x - width / 2, head.y);
    ImVec2 boxMax = ImVec2(head.x + width / 2, feet.y);
          // Health
          
    float health = (e.is_health < 0.0f) ? 0.0f : (e.is_health > 100.0f) ? 100.0f : e.is_health;
   // float health = std::clamp(e.is_health, 0.0f, 100.0f);
    float hpPercent = health / 100.0f;
    float barW = 4.0f;
    ImVec2 barTop = ImVec2(boxMin.x - 6.0f, boxMin.y);
    ImVec2 barBottom = ImVec2(barTop.x + barW, boxMin.y + height);
    ImVec2 fillTop = ImVec2(barTop.x, barTop.y + height * (1.0f - hpPercent));
    ImVec2 fillBottom = ImVec2(barBottom.x, barBottom.y);
    ImU32 hpColor = IM_COL32((1.0f - hpPercent) * 255, hpPercent * 255, 0, 255);
    draw->AddRectFilled(fillTop, fillBottom, hpColor);
    draw->AddRect(barTop, barBottom, IM_COL32(0, 0, 0, 255));
}

//ImVec2 To(ToImVec2(e.bones.at("head")));  
if (sync_bool["bESP_Box"] && !e.bones.empty()) {
    ImVec2 head = ToImVec2(e.bones.at("head"));
    auto foot_l = ToImVec2(e.bones.at("limbs_l_calf"));
    auto foot_r = ToImVec2(e.bones.at("limbs_r_calf"));

    ImVec2 feet = (foot_l.y > foot_r.y) ? foot_l : foot_r;

    float height = feet.y - head.y;
    float width = height * 0.6f;

    ImVec2 boxMin(head.x - width / 2, head.y);
    ImVec2 boxMax(head.x + width / 2, feet.y);

    // Ganti warna menjadi ungu (dengan alpha untuk transparansi)
    ImColor fillColor = e.is_visible ? ImColor(128, 0, 128, 90) : ImColor(128, 0, 128, 40);
    ImColor borderColor = e.is_visible ? ImColor(128, 0, 128, 255) : ImColor(128, 0, 128, 120);
    float thickness = sync_float["fESP_LineThickness"];

    draw->AddRectFilled(boxMin, boxMax, fillColor);
    // Jika ingin garis tepi, aktifkan baris ini
    // draw->AddRect(boxMin, boxMax, borderColor, 0, 0, thickness);
}


if (sync_bool["bESP_DangerCircle"] && e.is_visible < 15.0f) {
    draw->AddCircle(To, 30.0f, IM_COL32(255, 0, 0, 180), 32, 2.0f);
    draw->AddText(ImVec2(To.x - 20, To.y + 25), IM_COL32(255, 0, 0, 255), "DANGER!");
}


}
    
 if (players > 0 || bots > 0) {
    auto drawList = ImGui::GetForegroundDrawList();

    // Background rectangles
    drawList->AddRectFilled({Width / 2 - 170, 80}, {Width / 2 - 120, 120}, ImColor(60, 0, 90, 220), 5, 0);
    drawList->AddRectFilled({Width / 2 - 120, 80}, {Width / 2 - 50, 120}, ImColor(200, 150, 255, 220), 5, 0);

    drawList->AddRectFilled({Width / 2 + 50, 80}, {Width / 2 + 100, 120}, ImColor(60, 0, 90, 220), 5, 0);
    drawList->AddRectFilled({Width / 2 + 100, 80}, {Width / 2 + 170, 120}, ImColor(200, 150, 255, 220), 5, 0);

    // Text (centered)
    drawList->AddText(NULL, 35, {Width / 2 - 145 - (ImGui::CalcTextSize2(to_string(players).c_str(), 0, 35).x / 2), 82},
                      ImColor(255, 255, 255), to_string(players).c_str());
    drawList->AddText(NULL, 30, {Width / 2 - 83 - (ImGui::CalcTextSize2("Player", 0, 30).x / 2), 82},
                      ImColor(255, 255, 255), "Player");

    drawList->AddText(NULL, 35, {Width / 2 + 75 - (ImGui::CalcTextSize2(to_string(bots).c_str(), 0, 35).x / 2), 82},
                      ImColor(255, 255, 255), to_string(bots).c_str());
    drawList->AddText(NULL, 30, {Width / 2 + 135 - (ImGui::CalcTextSize2("Bots", 0, 30).x / 2), 82},
                      ImColor(255, 255, 255), "Bots");
} else {
    auto drawList = ImGui::GetForegroundDrawList();

    drawList->AddRectFilled({Width / 2 - 170, 80}, {Width / 2 - 120, 120}, ImColor(60, 0, 90, 220), 5, 0);
    drawList->AddRectFilled({Width / 2 - 120, 80}, {Width / 2 - 50, 120}, ImColor(200, 150, 255, 220), 5, 0);

    drawList->AddRectFilled({Width / 2 + 50, 80}, {Width / 2 + 100, 120}, ImColor(60, 0, 90, 220), 5, 0);
    drawList->AddRectFilled({Width / 2 + 100, 80}, {Width / 2 + 170, 120}, ImColor(200, 150, 255, 220), 5, 0);

    drawList->AddText(NULL, 35, {Width / 2 - 145 - (ImGui::CalcTextSize2(to_string(players).c_str(), 0, 35).x / 2), 82},
                      ImColor(255, 255, 255), to_string(players).c_str());
    drawList->AddText(NULL, 30, {Width / 2 - 83 - (ImGui::CalcTextSize2("Player", 0, 30).x / 2), 82},
                      ImColor(255, 255, 255), "Player");

    drawList->AddText(NULL, 35, {Width / 2 + 75 - (ImGui::CalcTextSize2(to_string(bots).c_str(), 0, 35).x / 2), 82},
                      ImColor(255, 255, 255), to_string(bots).c_str());
    drawList->AddText(NULL, 30, {Width / 2 + 135 - (ImGui::CalcTextSize2("Bots", 0, 30).x / 2), 82},
                      ImColor(255, 255, 255), "Bots");
}


 
        if (!sync_bool["bAIM_NoTargetHideFov"] || (players || bots)) {
               
        if (sync_bool["bAIM_DrawFov"]) draw->AddCircle(ImVec2(Width / 2, Height / 2), sync_float["fAIM_Fov"], WHITE, 0, sync_float["fESP_LineThickness"]);
     //   if (players || bots) { draw->drawText(("P:"+to_string(players)+" B:"+to_string(bots)).c_str(), 70, ImVec2(Width / 2, 120), CYAN); }
      //  else { draw->drawText("SAFE", 70, ImVec2(Width / 2, 120), WHITE); }
    } else { //draw->drawText("SAFE", 70, ImVec2(Width / 2, 120), WHITE); 
    }
}
void DrawMenu(ImGuiIO &io) {
    SetNextWindowSizeConstraints(ImVec2(800, 400), ImVec2(FLT_MAX, FLT_MAX));
    SetNextWindowSize(ImVec2(1000, 700), ImGuiCond_FirstUseEver);
    bool need_sync = false, tab_changed = false;

    sync_float["fHeight"] = Height;
    sync_float["fWidth"] = Width;
	
    // sync_with_py();
   static bool isLogin = false, logginIn = false;
static std::string err;

if (!Zaijan) {
ImGui::Text(OBFUSCATE("FUBUKI"));
    
}

if (!g_Token.empty() && !g_Auth.empty() && g_Token == g_Auth) {
bool Zaijan = false;
}
//LOGIN PAGE l SE7ENSINS
if (!isLogin) {
if (ImGui::Begin(oxorany("LOGIN PAGE"),  NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) 
	{
	//background animation start
	
	//=background end animation
           ImGui::Text(oxorany("  BUY KEY l @FUBUKIEX "));
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
            
            if (ImGui::Button(OBFUSCATE("LOGIN"), ImGui::PopItemWidth();
            if (!err.empty() && err != std::string(OBFUSCATE("OK"))) {
                
            }
            ImGui::PopItemWidth();
                }
		ImGui::Text("Device Info:");
               ImGui::Text("Device Model: %s", getDeviceName().c_str());
               ImGui::Text("CPU Architecture: %s", getDeviceArchitecture().c_str());
            ImGui::PopItemWidth();
            } else {
	static std::string current_tab = "";
	
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
    if (features["bAlwaysAutoResize"]) window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
    auto window_name = O("Fubuki Modsz Bloodstrike_Cheat @FubukiEx").chr();
    if (Begin(window_name, nullptr, window_flags)) {
        TrackImGuiWindow(window_name);
		
	   float time = ImGui::GetTime();

// Color logic: rainbow before 2 seconds, violet after
int r = 0, g = 0, b = 0;
if (time < 2.0f) {
    r = (int)(sin(time * 3.0f) * 127 + 128);
    g = (int)(sin(time * 3.0f + 3.0f) * 127 + 128);
    b = (int)(sin(time * 3.0f + 4.0f) * 127 + 128);
} else {
    r = 238;
    g = 130;
    b = 238;
}
  ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(oxorany(" CHANNLE | FUBUKI MODSZ ")).x) * 0.5f);
ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(r, g, b, 255));
ImGui::Text(oxorany("  DEVELOPER l @FubukiEx"));
ImGui::PopStyleColor(); 
ImGui::Separator();  
ImGui::SameLine();
    ImGui::Text(OBFUSCATE("BYPASS  [ RUNNING...%c ]"), "||//~~\\"[(int)(ImGui::GetTime() / 0.05f) & 3]);
    static float values[100]    = {};
    static int values_offset   = 0;
    static double refresh_time = 0.0;
    while( refresh_time < ImGui::GetTime() ) // Create data at fixed 60 Hz rate for the demo
    {
    static float phase    = 0.0f;
    values[values_offset] = cosf( phase );
    values_offset = ( values_offset + 1 ) % IM_ARRAYSIZE( values );
    phase += 0.10f * values_offset;
    refresh_time += 1.0f / 60.0f;
    }
    
    char overlay[60] = "";
    ImGui::PlotLines("", values, IM_ARRAYSIZE( values ), values_offset, overlay, -1.0f, 1.0f, ImVec2( 0,20.0f ) );
    ImGui::SameLine();
    ImGui::Text(OBFUSCATE("( FPS | %.1f )"), 1000.0f / ImGui::GetIO().Framerate);

                if (BeginTabBar("##TabBar")) {
            static bool color_changed = false;
    
            if (BeginTabItem("aimbot")) {
              if (current_tab != "aimbot") { current_tab = "aimbot"; tab_changed = true; }  
			    /*  if (sync_bool["bAIM"])
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f)); // Hijau saat ON
else
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f)); // Merah saat OFF

if (ImGui::Button(sync_bool["bAIM"] ? "ON" : "OFF")) {
    sync_bool["bAIM"] = !sync_bool["bAIM"];
    need_sync = true;
}

ImGui::PopStyleColor();*/
/*    if (sync_bool["bAIM"])
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.8f, 0.1f, 1.0f)); // Green
    if (ImGui::Button("disable aimbot", ImVec2(-1, 80)))
    {
        sync_bool["bAIM"] = false;
    }
    ImGui::PopStyleColor();
}
else
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f)); // Red
    if (ImGui::Button("enable aimbot", ImVec2(-1, 80)))
    {
        sync_bool["bAIM"] = true;
    }
    ImGui::PopStyleColor();
}*/
               need_sync |= Checkbox("enable", &sync_bool["bAIM"]);
                need_sync |= Checkbox("check visibility", &sync_bool["bAIM_CheckVisibility"]);
                need_sync |= Checkbox("ignore knocked", &sync_bool["bAIM_IgnoreKnocked"]);
                need_sync |= Checkbox("ignore bots", &sync_bool["bAIM_IgnoreBots"]);
              /*need_sync |= Checkbox("draw snapline", &sync_bool["bAIM_SnapLine"]);
if (sync_bool["bAIM_SnapLine"]) {
                    need_sync |= SliderFloat("SanpLine Radius", &sync_float["fAimbotFov"], 50.0f, 100.0f, "%.2f");
                }*/
                need_sync |= SliderFloat("snap strength", &sync_float["fAIM_SnapStrength"], 0.001f, 0.90f, "%.2f");
                need_sync |= Checkbox("draw fov", &sync_bool["bAIM_DrawFov"]);
                need_sync |= SliderFloat("fov", &sync_float["fAIM_Fov"], 30.0f, 1500.0f, "%.2f");
                need_sync |= Checkbox("hide fov if there's no target", &sync_bool["bAIM_NoTargetHideFov"]);
                need_sync |= Combo("trigger", &sync_int["iAIM_Trigger"], aimbotTrigger, IM_ARRAYSIZE(aimbotTrigger));
                need_sync |= Combo("target bone", &sync_int["iAIM_TargetBone"], targetBone, IM_ARRAYSIZE(targetBone));
                
                EndTabItem();
            }	
		
            if (BeginTabItem("esp")) {
				if (current_tab != "esp") { current_tab = "esp"; tab_changed = true; }
				if (sync_bool["bESP"])
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.8f, 0.1f, 1.0f)); // Green
    if (ImGui::Button("ESP DISABLE", ImVec2(-1, 80)))
    {
        sync_bool["bESP"] = false;
    }
    ImGui::PopStyleColor();
}
else
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f)); // Red
    if (ImGui::Button("ESP ENABLE", ImVec2(-1, 80)))
    {
        sync_bool["bESP"] = true;
    }
    ImGui::PopStyleColor();
   }
               // need_sync |= Checkbox("enable", &sync_bool["bESP"]);
				need_sync |= Combo("starting point", &sync_int["iESP_Point"], espPoint, IM_ARRAYSIZE(espPoint));
                need_sync |= Checkbox("esp line", &sync_bool["bESP_Line"]);
                if (sync_bool["bESP_Line"]) {
                    SameLine();
                    color_changed |= ImGui::ColorEdit4("##line color", &colors["cESP_Line"].Value.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip);
                    SameLine();
                    color_changed |= ImGui::ColorEdit4("##line hidden color", &colors["cESP_LineHidden"].Value.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip);
                }
                
                need_sync |= Checkbox("esp line for bots", &sync_bool["bESP_LineBots"]);
                if (sync_bool["bESP_LineBots"]) {
                    SameLine();
                    color_changed |= ImGui::ColorEdit4("##line bots color", &colors["cESP_LineBots"].Value.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip);
                    SameLine();
                    color_changed |= ImGui::ColorEdit4("##line bots hidden color", &colors["cESP_LineBotsHidden"].Value.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip);
                }
                
                need_sync |= Checkbox("esp skeleton", &sync_bool["bESP_Skeleton"]);
                if (sync_bool["bESP_Skeleton"]) {
                    SameLine();
                    color_changed |= ImGui::ColorEdit4("##skeleton color", &colors["cESP_Skeleton"].Value.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip);
                    SameLine();
                    color_changed |= ImGui::ColorEdit4("##skeleton hidden color", &colors["cESP_SkeletonHidden"].Value.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip);
                }
                
              /*  need_sync |= Checkbox("Esp Healt", &sync_bool["bESP_Health"]);       
				NextColumn();*/
				
                need_sync |= Checkbox("esp Box", &sync_bool["bESP_Box"]); 		
                need_sync |= Checkbox("esp Distance", &sync_bool["bESP_Distance"]);             
                need_sync |= Checkbox("esp allert", &sync_bool["bESP_Name"]);				
	       //   need_sync |= Checkbox("esp teamId", &sync_bool["bESP_TeamID"]);
                need_sync |= SliderFloat("line thickness", &sync_float["fESP_LineThickness"], 1.0f, 10.0f, "%.2f");
                
                EndTabItem();
            }
            
            
           if (BeginTabItem("memory")) {
            // Kontol
			if (current_tab != "memory") { current_tab = "memory"; tab_changed = true; }
			need_sync |= Checkbox("SkinHack", &sync_bool["bSkinHack"]);
            need_sync |= Checkbox("NoRecoil v1", &sync_bool["bFeature1"]);
            if (sync_bool["bFeature1"] && !last_sync_bool["bFeature1"]) {
                var.Feature1.Modify();
            } else if (!sync_bool["bFeature1"] && last_sync_bool["bFeature1"]) {
                var.Feature1.Restore();   
            }
		    need_sync |= Checkbox("NoRecoil v2", &sync_bool["bNoRecoil"]);
			need_sync |= Checkbox("NoSpread", &sync_bool["bNoSpread"]);
            need_sync |= Checkbox("wall hack", &sync_bool["bXray"]);
            need_sync |= Checkbox("SpeedHack", &sync_bool["bSpeed"]);
                SameLine();
            need_sync |= SliderFloat("", &sync_float["fSpeed"], 1.0f, 1.5f, "%.2f", ImGuiSliderFlags_None);
                
                      
            EndTabItem();
          last_sync_bool = sync_bool;
        }   
            if (BeginTabItem("price")) {
               if (current_tab != "price") { current_tab = "price"; tab_changed = true; }
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(r, g, b, 255));
ImGui::Text(oxorany("   3 day | 50.000 idr / 3$ usd "));
ImGui::PopStyleColor(); 
ImGui::Separator();  
  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(r, g, b, 255));
ImGui::Text(oxorany("   7 day | 80.000 idr / 5$ usd "));
ImGui::PopStyleColor(); 
ImGui::Separator();  
ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(r, g, b, 255));
ImGui::Text(oxorany("   14 day | 125.000 idr / 8$ usd "));
ImGui::PopStyleColor(); 
ImGui::Separator();  
ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(r, g, b, 255));
ImGui::Text(oxorany("   30 day | 150.000 idr / 10$ usd "));
ImGui::PopStyleColor(); 
ImGui::Separator();  
ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(r, g, b, 255));
ImGui::Text(oxorany("   60 day | 200.000 idr / 15$ usd "));
ImGui::PopStyleColor(); 
ImGui::Separator();  

                EndTabItem();
            }
  
	if (BeginTabItem("style")) {
                if (current_tab != "style") { current_tab = "style"; tab_changed = true; }

                static bool style_changed = false;

                ImGuiStyle& style = GetStyle();

                bool theme_changed = ImGui::Combo("Theme", &current_theme, themes, IM_ARRAYSIZE(themes));
                if (theme_changed) switch_theme(current_theme);
                
                bool rounding_changed = ImGui::SliderFloat("Rounding", &Rounding, 0.0f, 20.0f);
                if (rounding_changed) {
                    style.FrameRounding = Rounding;
                    style.WindowRounding = Rounding;
                    style.ChildRounding = Rounding;
                    style.PopupRounding = Rounding;
                    style.ScrollbarRounding = Rounding;
                    style.GrabRounding = Rounding;
                    style.LogSliderDeadzone = Rounding;
                    style.TabRounding = Rounding;
                }

                style_changed |= theme_changed;
                style_changed |= rounding_changed;
                style_changed |= ImGui::SliderFloat("Alpha", &style.Alpha, 0.6f, 1.0f);
                style_changed |= ImGui::SliderFloat("BorderSize", &style.FrameBorderSize, 0.0f, 5.0f);
                style_changed |= ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 10.0f, 40.0f);
                
                if (style_changed) save_imgui_style(&style);

                EndTabItem();
            }

         if (BeginTabItem("settings")) {
                             bool settings_changed = false;
                settings_changed |= Checkbox("AlwaysAutoResize", &features["bAlwaysAutoResize"]);
                settings_changed |= Checkbox("RememberSettings (restart required)", &features["bImguiAutoSave"]);
                settings_changed |= SliderFloat("FontScale (restart required)", &features_float["fFontScale"], 30.0f, 60.0f);
                settings_changed |= Checkbox("Better Touch (May crash)", &_features["bBetterTouch"]);
                
                if (settings_changed) save_features();
                
                EndTabItem();
            }
            

            EndTabBar();
        }

        if (tab_changed || need_sync) sync_with_py();

    } End();
}
}

static bool setup = false;
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

    HOOKIT("libEGL.so", "eglSwapBuffers", Draw);
    
    LOGI("RETURNING FROM __IMGUI__");
}
