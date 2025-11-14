#include "rendering/core/hud.h"
#include "raylib.h"

namespace rendering {

HUD::HUD()
    : m_title("Yoshi's Wrath - BSP Engine")
    , m_controls_text("WASD: Move | Mouse: Look | ESC: Exit")
    , m_show_fps(true)
    , m_title_font_size(20)
    , m_controls_font_size(16)
    , m_title_color(GREEN)
    , m_controls_color(LIGHTGRAY) {
}

void HUD::render() {
    // Render title at top-left
    DrawText(m_title.c_str(), 10, 10, m_title_font_size, m_title_color);

    // Render controls text below title
    DrawText(m_controls_text.c_str(), 10, 40, m_controls_font_size, m_controls_color);

    // Render FPS counter at bottom-left if enabled
    if (m_show_fps) {
        DrawFPS(10, GetScreenHeight() - 30);
    }
}

void HUD::set_title(const std::string& title) {
    m_title = title;
}

void HUD::set_controls_text(const std::string& controls) {
    m_controls_text = controls;
}

void HUD::set_show_fps(bool show) {
    m_show_fps = show;
}

} // namespace rendering
