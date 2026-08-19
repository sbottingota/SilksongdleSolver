#include "application.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include "log_window.h"

#include <iostream>
#include <vector>
#include <cstring>

const size_t buffer_size = 64;

const float row_height = 32.0f;

constexpr ImVec4 button_color = ImVec4(0.0, 0.0, 0.0, 0.0); // transparent

// varying degrees of visibility
constexpr ImVec4 button_hover_color = ImVec4(0.3, 0.3, 0.7, 0.1);
constexpr ImVec4 button_selected_color = ImVec4(0.3, 0.3, 0.7, 0.3);


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
    FieldColor color = FieldColor::None;
    FieldColor health = FieldColor::None;
    FieldColor kill_count = FieldColor::None;
};

static std::vector<GuessRow> rows;
static FieldColor *selected_field = nullptr;

static ImU32 get_field_color(FieldColor color) {
    switch (color) {
        case FieldColor::Red:
        return IM_COL32(255, 50, 50, 255);

        case FieldColor::Green:
        return IM_COL32(50, 200, 50, 255);

        case FieldColor::Orange:
        return IM_COL32(255, 128, 50, 255);

        case FieldColor::None:
        default:
        return IM_COL32(80, 80, 80, 255);
    }
}

static void draw_field(const char *label, FieldColor *field, bool interactive_fields) {
    ImU32 color = get_field_color(*field);

    float width = ImGui::GetContentRegionAvail().x;

    ImVec2 pos = ImGui::GetCursorScreenPos();
    float cell_width = ImGui::GetContentRegionAvail().x;

    ImGui::GetWindowDrawList()->AddRectFilled(
        pos, ImVec2(pos.x + cell_width, pos.y + row_height), color
    );

    // add transparent button overlaid onto the cell to check for clicks
    if (interactive_fields) {
        // if the current field is selected, show as such
        if (selected_field == field) {
            ImGui::PushStyleColor(ImGuiCol_Button, button_selected_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_selected_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_selected_color);
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, button_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hover_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_selected_color);
        }


        if (ImGui::Button(label, ImVec2(width, row_height))) {
            selected_field = field;
        }

        ImGui::PopStyleColor(3);

    } else {
        // ImGui::TextUnformatted("↑↓");
    }
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

        int rows_end = rows.size() - 1;

        // note that rows[0] is the *last* row, and rows[rows.size() - 1] is the *first* row
        for (int i = rows_end; i >= 0; --i) {
            GuessRow &row = rows[i];

            bool interactive_fields = i == rows_end;

            ImGui::PushID(i);

            std::string idx = std::to_string(i);

            ImGui::TableNextRow(ImGuiTableRowFlags_None, row_height);

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(row.name.c_str());

            ImGui::TableNextColumn();
            draw_field("##type", &row.type, interactive_fields);

            ImGui::TableNextColumn();
            draw_field("##location", &row.location, interactive_fields);

            ImGui::TableNextColumn();
            draw_field("##color", &row.color, interactive_fields);

            ImGui::TableNextColumn();
            draw_field("##health", &row.health, interactive_fields);

            ImGui::TableNextColumn();
            draw_field("##kill_count", &row.kill_count, interactive_fields);

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

void draw_guess_gui() {
    ImGui::Begin("Silksongdle Solver");

    static char guess_buffer[buffer_size] = {0};

    ImGui::InputText("##guess", guess_buffer, buffer_size);
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
        // for each key, set the selected field the specified color, and then deselect that field

        if (ImGui::IsKeyPressed(ImGuiKey_R)) {
            *selected_field = FieldColor::Red;
            selected_field = nullptr;

        } else if (ImGui::IsKeyPressed(ImGuiKey_O)) {
            *selected_field = FieldColor::Orange;
            selected_field = nullptr;

        } else if (ImGui::IsKeyPressed(ImGuiKey_G)) {
            *selected_field = FieldColor::Green;
            selected_field = nullptr;

        } else if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
            selected_field = nullptr; // just deselect current selection
        }
    }

    ImGui::End();
}

void draw_app_frame() {
    static bool a = true;

    if (a) {
        log("Hello, world!");
        log("Logging window");
        a = false;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    draw_guess_gui();
    draw_log_window();

    ImGui::Render();
}

