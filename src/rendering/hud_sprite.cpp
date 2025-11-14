#include "rendering/hud_sprite.h"
#include "platform/file_system.h"
#include "raylib.h"
#include <fstream>

namespace rendering {

HUDSprite::HUDSprite()
    : BaseSprite()
    , m_position{0.0f, 0.0f}
    , m_scale(2.0f) {
}

bool HUDSprite::load_from_json(const std::string& json_path) {
    // Load JSON file using platform-agnostic path
    std::string full_path = platform::FileSystem::join_path(
        platform::FileSystem::get_assets_path(), json_path);
    std::ifstream file(full_path);
    if (!file.is_open()) {
        return false;
    }

    // Simple JSON parsing for our specific format
    std::string atlas_path;
    int frame_width = 0, frame_height = 0;
    int frames_h = 0, frames_v = 0;

    std::string line;
    while (std::getline(file, line)) {
        // Parse atlas path
        if (line.find("\"atlas\"") != std::string::npos) {
            size_t start = line.find(": \"") + 3;
            size_t end = line.find("\"", start);
            atlas_path = line.substr(start, end - start);
        }
        // Parse frame_width
        else if (line.find("\"frame_width\"") != std::string::npos) {
            size_t start = line.find(": ") + 2;
            frame_width = std::stoi(line.substr(start));
        }
        // Parse frame_height
        else if (line.find("\"frame_height\"") != std::string::npos) {
            size_t start = line.find(": ") + 2;
            frame_height = std::stoi(line.substr(start));
        }
        // Parse frames_horizontal
        else if (line.find("\"frames_horizontal\"") != std::string::npos) {
            size_t start = line.find(": ") + 2;
            frames_h = std::stoi(line.substr(start));
        }
        // Parse frames_vertical
        else if (line.find("\"frames_vertical\"") != std::string::npos) {
            size_t start = line.find(": ") + 2;
            frames_v = std::stoi(line.substr(start));
        }
    }

    file.close();

    // Load texture atlas
    if (!m_atlas->load(atlas_path, frame_width, frame_height, frames_h, frames_v)) {
        return false;
    }

    // Set default position to center-bottom of screen
    m_position.x = GetScreenWidth() / 2.0f;
    m_position.y = GetScreenHeight() - (frame_height * m_scale / 2.0f);

    return true;
}

void HUDSprite::render() {
    if (!m_atlas->is_loaded()) {
        return;
    }

    // Get current frame from animation
    int frame_index = m_animation_controller.get_current_frame();
    Rectangle source_rect = m_atlas->get_frame_uv(frame_index);

    // Calculate destination rectangle (centered on position)
    float scaled_width = m_atlas->get_frame_width() * m_scale;
    float scaled_height = m_atlas->get_frame_height() * m_scale;

    Rectangle dest_rect;
    dest_rect.x = m_position.x - scaled_width / 2.0f;
    dest_rect.y = m_position.y - scaled_height / 2.0f;
    dest_rect.width = scaled_width;
    dest_rect.height = scaled_height;

    // Draw sprite
    DrawTexturePro(m_atlas->get_texture(), source_rect, dest_rect,
                   {0.0f, 0.0f}, 0.0f, WHITE);
}

void HUDSprite::set_position(const Vector2& position) {
    m_position = position;
}

void HUDSprite::set_scale(float scale) {
    m_scale = scale;
}

} // namespace rendering
