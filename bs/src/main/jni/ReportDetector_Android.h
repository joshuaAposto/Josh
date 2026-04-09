#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <android/log.h>
#include "imgui.h"
#include "xorstr.hpp"
#include "curl/json.hpp"

#define REPORT_LOGI(...) __android_log_print(ANDROID_LOG_INFO, "REPORT_DETECTOR", __VA_ARGS__)
#define REPORT_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "REPORT_DETECTOR", __VA_ARGS__)

using json = nlohmann::json;

// ========== STRUCT REPORT DATA ==========
struct ReportData {
    std::string reporterID;
    std::string reporterName;
    std::string targetID;
    std::string reason;
    std::string category;
    time_t timestamp;
    int severity;
    
    ReportData() : severity(1), timestamp(0) {}
};

// ========== CLASS REPORT DETECTOR ==========
class ReportDetector {
private:
    std::vector<ReportData> history;
    bool enabled;
    int totalReports;
    std::thread fifoThread;
    bool running;
    char searchFilter[128];
    
    void readFifoLoop() {
        const char* FIFO_PATH = "/data/data/com.netease.newspike/imgui/report";
        
        // Create FIFO if not exists
        mkfifo(FIFO_PATH, 0666);
        
        while (running) {
            int fd = open(FIFO_PATH, O_RDONLY | O_NONBLOCK);
            if (fd < 0) {
                usleep(100000); // 100ms
                continue;
            }
            
            char buffer[4096];
            ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
            
            if (bytes > 0) {
                buffer[bytes] = '\0';
                std::string data(buffer);
                
                // Split by newlines
                size_t pos = 0;
                while ((pos = data.find('\n')) != std::string::npos) {
                    std::string line = data.substr(0, pos);
                    if (!line.empty()) {
                        processReportData(line);
                    }
                    data.erase(0, pos + 1);
                }
            }
            
            close(fd);
            usleep(50000); // 50ms
        }
    }
    
    void processReportData(const std::string& jsonStr) {
        try {
            json j = json::parse(jsonStr);
            
            if (!j["detected"].get<bool>()) return;
            
            ReportData report;
            report.reporterID = j["reporter_id"].get<std::string>();
            report.reporterName = j["reporter_name"].get<std::string>();
            report.targetID = j["target_id"].get<std::string>();
            report.reason = j["reason"].get<std::string>();
            report.category = j["category"].get<std::string>();
            report.timestamp = j["timestamp"].get<time_t>();
            report.severity = j["severity"].get<int>();
            
            history.push_back(report);
            totalReports++;
            
            REPORT_LOGE("🚨 REPORT DETECTED!");
            REPORT_LOGE("   Reporter: %s (ID: %s)", report.reporterName.c_str(), report.reporterID.c_str());
            REPORT_LOGE("   Reason: %s", report.reason.c_str());
            
            // Save to file
            FILE* logFile = fopen("/storage/emulated/0/report_history.txt", "a");
            if (logFile) {
                fprintf(logFile, "[%s] Reporter: %s | Reason: %s\n",
                       timeToString(report.timestamp).c_str(),
                       report.reporterName.c_str(),
                       report.reason.c_str());
                fclose(logFile);
            }
            
        } catch (const std::exception& e) {
            REPORT_LOGE("Failed to parse: %s", e.what());
        }
    }
    
    std::string timeToString(time_t time) {
        char buffer[64];
        struct tm* timeinfo = localtime(&time);
        strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
        return std::string(buffer);
    }
    
    ImVec4 getSeverityColor(int severity) {
        switch (severity) {
            case 1: return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            case 2: return ImVec4(0.5f, 1.0f, 0.0f, 1.0f);
            case 3: return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            case 4: return ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
            case 5: return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            default: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
    
public:
    ReportDetector() : enabled(true), totalReports(0), running(false) {
        memset(searchFilter, 0, sizeof(searchFilter));
    }
    
    ~ReportDetector() {
        stop();
    }
    
    void start() {
        if (running) return;
        running = true;
        fifoThread = std::thread(&ReportDetector::readFifoLoop, this);
        fifoThread.detach();
        REPORT_LOGI("Report Detector Started");
    }
    
    void stop() {
        running = false;
    }
    
    void clearHistory() {
        history.clear();
        totalReports = 0;
    }
    
    int getTotalReports() const { return totalReports; }
    bool isEnabled() const { return enabled; }
    
    void renderMenu() {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize;
        ImGui::SetNextWindowSizeConstraints(ImVec2(820, 0), ImVec2(840, FLT_MAX));
        
        if (ImGui::Begin(xorstr_("🚨 Report Detector"), nullptr, window_flags)) {
            
            // Header
            static float pulse = 0.0f;
            pulse += 0.05f;
            float intensity = 0.5f + 0.5f * sin(pulse);
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f * intensity, 0.0f, 0.0f, 1.0f));
            ImGui::Text("🚨 REPORT DETECTION SYSTEM");
            ImGui::PopStyleColor();
            
            ImGui::Separator();
            
            // Status
            ImGui::Checkbox("Enable Detection", &enabled);
            ImGui::SameLine();
            if (enabled) {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "● ACTIVE");
            } else {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "● DISABLED");
            }
            
            ImGui::Separator();
            ImGui::Text("📊 Total Reports: %d", totalReports);
            ImGui::Separator();
            
            // Search
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 80);
            ImGui::InputTextWithHint("##search", "🔍 Search...", searchFilter, IM_ARRAYSIZE(searchFilter));
            ImGui::SameLine();
            if (ImGui::Button("Clear", ImVec2(70, 0))) {
                clearHistory();
            }
            
            ImGui::Separator();
            
            // History
            float availHeight = ImGui::GetContentRegionAvail().y - 60;
            ImGui::BeginChild("ReportHistory", ImVec2(0, availHeight), true, 
                             ImGuiWindowFlags_AlwaysVerticalScrollbar);
            
            if (history.empty()) {
                ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), 
                                  "No reports yet... You're safe!");
            } else {
                for (size_t i = history.size(); i > 0; i--) {
                    const auto& report = history[i-1];
                    ImVec4 color = getSeverityColor(report.severity);
                    
                    ImGui::PushStyleColor(ImGuiCol_Header, 
                        ImVec4(color.x * 0.3f, color.y * 0.3f, color.z * 0.3f, 0.8f));
                    
                    char label[256];
                    snprintf(label, sizeof(label), "🚫 [%s] %s###%zu", 
                            timeToString(report.timestamp).c_str(),
                            report.reporterName.c_str(), i);
                    
                    if (ImGui::CollapsingHeader(label)) {
                        ImGui::Indent(20.0f);
                        ImGui::TextColored(color, "📋 %s", report.category.c_str());
                        ImGui::Text("👤 ID: %s", report.reporterID.c_str());
                        ImGui::TextWrapped("💬 %s", report.reason.c_str());
                        ImGui::Unindent(20.0f);
                    }
                    
                    ImGui::PopStyleColor();
                    ImGui::Spacing();
                }
            }
            
            ImGui::EndChild();
            ImGui::Separator();
            
            ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), 
                              "⚠️ Log: /storage/emulated/0/report_history.txt");
            
            ImGui::End();
        }
    }
};

// Global instance
static ReportDetector g_ReportDetector;