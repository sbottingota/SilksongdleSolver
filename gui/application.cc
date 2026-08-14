#include "application.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <vector>
#include <cstring>

#define BUFFER_SIZE 64

enum class FieldColor {
    None,
    Red,
    Orange,
    Green
};

struct GuessRow {
    std::string name;
    FieldColor type = FieldColor::None;
    FieldColor location = FieldColor::None;
    FieldColor colour = FieldColor::None;
    FieldColor health = FieldColor::None;
    FieldColor kills = FieldColor::None;
};

static std::vector<GuessRow> rows;
static FieldColor *selected_field = nullptr;

static ImVec4 get_field_color(FieldColor color) {
    switch (color) {
        case FieldColor::Red:
        return ImVec4(1.0, 0.2, 0.2, 1.0);

        case FieldColor::Green:
        return ImVec4(0.2, 1.0, 0.2, 1.0);

        case FieldColor::Orange:
        return ImVec4(1.0, 0.2, 1.0, 1.0);

        case FieldColor::None:
        default:
        return ImVec4(0.3, 0.3, 0.3, 1.0);
    }
}

static void draw_field(const char *label, FieldColor *field) {
    ImVec4 color = get_field_color(*field);

    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);

    if (ImGui::Button(label)) {
        selected_field = field;
    }

    ImGui::PopStyleColor(3);
}

static void draw_guess_table() {
    if (rows.empty()) return;

    if (ImGui::BeginTable("guesses", 6,
        ImGuiTableFlags_Borders
        | ImGuiTableFlags_RowBg
        | ImGuiTableFlags_Resizable
        | ImGuiTableFlags_SizingStretchProp)) {

        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Location");
        ImGui::TableSetupColumn("Color");
        ImGui::TableSetupColumn("Health");
        ImGui::TableSetupColumn("Kill Count");
        ImGui::TableHeadersRow();

        for (size_t i = 0; i < rows.size(); ++i) {
            GuessRow &row = rows[i];

            ImGui::PushID(i);

            std::string idx = std::to_string(i);

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(row.name.c_str());

            ImGui::TableNextColumn();
            draw_field("##type", &row.type);

            ImGui::TableNextColumn();
            draw_field("##location", &row.type);

            ImGui::TableNextColumn();
            draw_field("##color", &row.type);

            ImGui::TableNextColumn();
            draw_field("##health", &row.type);

            ImGui::TableNextColumn();
            draw_field("##kill_count", &row.type);

            ImGui::PopID();
        }
    }

    ImGui::EndTable();
}

void draw_guess_gui() {
    static char guess_buffer[BUFFER_SIZE] = {0};

    ImGui::InputText("##guess", guess_buffer, BUFFER_SIZE);
    ImGui::SameLine();

    if (ImGui::Button("Enter Guess")) {
        if (guess_buffer[0] != '\0') {
            rows.push_back({.name = guess_buffer});
            guess_buffer[0] = '\0';
        }
    }

    draw_guess_table();

    // keyboard handling
    if (selected_field != nullptr) {
        if (ImGui::IsKeyPressed(ImGuiKey_R)) {
            *selected_field = FieldColor::Red;

        } else if (ImGui::IsKeyPressed(ImGuiKey_O)) {
            *selected_field = FieldColor::Orange;

        } else if (ImGui::IsKeyPressed(ImGuiKey_G)) {
            *selected_field = FieldColor::Green;
        }
    }
}

void draw_app_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Silksongdle Solver");
    draw_guess_gui();
    ImGui::End();

    ImGui::Render();
}

