#include "rendering/sprites/billboard_sprite.h"
#include "platform/file_system.h"
#include "raymath.h"
#include "raylib.h"
#include "rlgl.h"
#include <fstream>

namespace rendering {

BillboardSprite::BillboardSprite()
    : BaseSprite()
    , m_position{0.0f, 0.0f, 0.0f}
    , m_width(1.0f)
    , m_height(1.0f)
    , m_anchor_y(0.0f) {
}

bool BillboardSprite::load_from_json(const std::string& json_path) {
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
    return m_atlas->load(atlas_path, frame_width, frame_height, frames_h, frames_v);
}

void BillboardSprite::render() {
    // Billboards require camera information, use render_with_camera() instead
    // This is here for interface compliance but does nothing
}

void BillboardSprite::render_with_camera(const game::Camera& camera) {
    if (!m_atlas->is_loaded()) {
        return;
    }

    // Get current frame from animation
    int frame_index = m_animation_controller.get_current_frame();
    Rectangle source_rect = m_atlas->get_frame_uv(frame_index);

    // Get camera vectors
    Vector3 cam_forward = camera.get_forward();
    Vector3 cam_right = Vector3CrossProduct(cam_forward, {0.0f, 1.0f, 0.0f});
    cam_right = Vector3Normalize(cam_right);

    // Calculate sprite quad corners (billboard facing camera)
    float half_width = m_width * 0.5f;
    float anchor_offset = m_height * m_anchor_y;

    Vector3 bottom_center = m_position;
    bottom_center.y += anchor_offset;

    Vector3 top_center = bottom_center;
    top_center.y += m_height;

    // Calculate quad corners
    Vector3 bottom_left = Vector3Subtract(bottom_center, Vector3Scale(cam_right, half_width));
    Vector3 bottom_right = Vector3Add(bottom_center, Vector3Scale(cam_right, half_width));
    Vector3 top_left = Vector3Subtract(top_center, Vector3Scale(cam_right, half_width));
    Vector3 top_right = Vector3Add(top_center, Vector3Scale(cam_right, half_width));

    // Draw textured quad manually
    rlSetTexture(m_atlas->get_texture().id);
    rlBegin(RL_QUADS);
    rlColor4ub(255, 255, 255, 255);

    rlTexCoord2f(source_rect.x / m_atlas->get_texture().width,
                 (source_rect.y + source_rect.height) / m_atlas->get_texture().height);
    rlVertex3f(bottom_left.x, bottom_left.y, bottom_left.z);

    rlTexCoord2f((source_rect.x + source_rect.width) / m_atlas->get_texture().width,
                 (source_rect.y + source_rect.height) / m_atlas->get_texture().height);
    rlVertex3f(bottom_right.x, bottom_right.y, bottom_right.z);

    rlTexCoord2f((source_rect.x + source_rect.width) / m_atlas->get_texture().width,
                 source_rect.y / m_atlas->get_texture().height);
    rlVertex3f(top_right.x, top_right.y, top_right.z);

    rlTexCoord2f(source_rect.x / m_atlas->get_texture().width,
                 source_rect.y / m_atlas->get_texture().height);
    rlVertex3f(top_left.x, top_left.y, top_left.z);

    rlEnd();
    rlSetTexture(0);
}

void BillboardSprite::set_position(const Vector3& position) {
    m_position = position;
}

void BillboardSprite::set_size(float width, float height) {
    m_width = width;
    m_height = height;
}

void BillboardSprite::set_anchor_y(float anchor) {
    m_anchor_y = anchor;
}

float BillboardSprite::get_distance_sq_from(const Vector3& point) const {
    float dx = m_position.x - point.x;
    float dy = m_position.y - point.y;
    float dz = m_position.z - point.z;
    return dx * dx + dy * dy + dz * dz;
}

} // namespace rendering
