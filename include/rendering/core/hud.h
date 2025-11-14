#pragma once

#include "raylib.h"
#include <string>

namespace rendering {

// HUD overlay for displaying game information
class HUD {
public:
    HUD();
    ~HUD() = default;

    // Render the HUD (call outside of 3D mode)
    void render();

    // Update HUD state
    void set_title(const std::string& title);
    void set_controls_text(const std::string& controls);
    void set_show_fps(bool show);

private:
    std::string m_title;
    std::string m_controls_text;
    bool m_show_fps;

    int m_title_font_size;
    int m_controls_font_size;
    Color m_title_color;
    Color m_controls_color;
};

} // namespace rendering
