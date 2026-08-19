#include "log_window.h"

#include "imgui.h"

#include <vector>
#include <string>

#include <cstdio>
#include <cstdarg>

static std::vector<std::string> logs;

void draw_log_window() {
    ImGui::Begin("Logs");

    if (ImGui::Button("Clear Logs")) {
        clear_logs();
    }

    ImGui::Separator();

    ImGui::BeginChild(
        "LogOutput",
        ImVec2(0, 0),
        ImGuiChildFlags_None,
        ImGuiWindowFlags_HorizontalScrollbar
    );

    for (const std::string &msg : logs) {
        ImGui::TextUnformatted(msg.c_str());
    }

    ImGui::EndChild();

    ImGui::End();
}

void log(const char *format, ...) {
    va_list args;
    va_start(args, format);

    // find out how large the formatted string should be
    va_list args_copy;
    va_copy(args_copy, args);

    int length = std::vsnprintf(nullptr, 0, format, args_copy);
    va_end(args_copy);

    if (length < 0) {
        va_end(args);
        return;
    }

    std::string msg(length, '\0');

    std::vsnprintf(msg.data(), msg.size() + 1, format, args);
    va_end(args);

    logs.push_back(std::move(msg));
}

void clear_logs() {
    logs.clear();
}

