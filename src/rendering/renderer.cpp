#include "rendering/renderer.h"
#include "game/bsp.h"
#include "raymath.h"
#include "rlgl.h"
#include <algorithm>

namespace rendering {

BasicRenderer::BasicRenderer()
    : m_texture_manager(std::make_unique<TextureManager>()) {
}

void BasicRenderer::begin_frame() {
    BeginDrawing();
    ClearBackground(BLACK);
}

void BasicRenderer::end_frame() {
    EndDrawing();
}

void BasicRenderer::render(const game::Level& level, const game::Camera& camera) {
    Camera3D raylib_camera = camera.to_raylib_camera();

    BeginMode3D(raylib_camera);

    const auto& sectors = level.get_sectors();

    // Use BSP tree for optimized rendering
    std::vector<uint32_t> visible_sectors;
    level.get_bsp_tree()->get_visible_sectors(camera.get_position(), visible_sectors);

    // Render visible sectors in BSP order
    for (uint32_t idx : visible_sectors) {
        if (idx < sectors.size()) {
            render_sector(sectors[idx]);
        }
    }

    EndMode3D();

    // Draw HUD
    DrawText("Yoshi's Wrath - BSP Engine", 10, 10, 20, GREEN);
    DrawText("WASD: Move | Mouse: Look | ESC: Exit", 10, 40, 16, LIGHTGRAY);
    DrawFPS(10, GetScreenHeight() - 30);
}

void BasicRenderer::render_sector(const game::Sector& sector) {
    // Render walls first
    for (const auto& wall : sector.walls) {
        render_wall(sector, wall);
    }

    // Then floor/ceiling
    render_floor_ceiling(sector);
}

void BasicRenderer::render_wall(const game::Sector& sector, const game::Wall& wall) {
    // Skip walls that are portals (they're openings, not solid walls)
    if (wall.portal_id >= 0) {
        return;
    }

    const game::Vertex& v1 = sector.vertices[wall.vertex_a];
    const game::Vertex& v2 = sector.vertices[wall.vertex_b];

    // Wall corners
    Vector3 bottom_left = {v1.x, sector.floor_height, v1.z};
    Vector3 bottom_right = {v2.x, sector.floor_height, v2.z};
    Vector3 top_left = {v1.x, sector.ceiling_height, v1.z};
    Vector3 top_right = {v2.x, sector.ceiling_height, v2.z};

    // Get texture
    const Texture2D& texture = m_texture_manager->get_texture(wall.texture_id);

    // Calculate wall length for UV scaling
    float wall_length = sqrtf(
        (v2.x - v1.x) * (v2.x - v1.x) +
        (v2.z - v1.z) * (v2.z - v1.z)
    );
    float wall_height = sector.ceiling_height - sector.floor_height;

    // Draw textured quad
    draw_textured_quad(bottom_left, bottom_right, top_right, top_left,
                      texture, wall_length, wall_height);

    // Draw wireframe outline for debugging (optional)
    // DrawLine3D(bottom_left, bottom_right, DARKGRAY);
    // DrawLine3D(bottom_left, top_left, DARKGRAY);
    // DrawLine3D(top_left, top_right, DARKGRAY);
    // DrawLine3D(bottom_right, top_right, DARKGRAY);
}

void BasicRenderer::render_floor_ceiling(const game::Sector& sector) {
    // For now, render as a simple quad (assumes rectangular room)
    if (sector.vertices.size() != 4) {
        return;  // Skip non-rectangular sectors for now
    }

    // Get textures
    const Texture2D& floor_tex = m_texture_manager->get_texture(sector.floor_texture);
    const Texture2D& ceil_tex = m_texture_manager->get_texture(sector.ceiling_texture);

    // Floor corners
    Vector3 floor_v0 = {sector.vertices[0].x, sector.floor_height, sector.vertices[0].z};
    Vector3 floor_v1 = {sector.vertices[1].x, sector.floor_height, sector.vertices[1].z};
    Vector3 floor_v2 = {sector.vertices[2].x, sector.floor_height, sector.vertices[2].z};
    Vector3 floor_v3 = {sector.vertices[3].x, sector.floor_height, sector.vertices[3].z};

    // Ceiling corners
    Vector3 ceil_v0 = {sector.vertices[0].x, sector.ceiling_height, sector.vertices[0].z};
    Vector3 ceil_v1 = {sector.vertices[1].x, sector.ceiling_height, sector.vertices[1].z};
    Vector3 ceil_v2 = {sector.vertices[2].x, sector.ceiling_height, sector.vertices[2].z};
    Vector3 ceil_v3 = {sector.vertices[3].x, sector.ceiling_height, sector.vertices[3].z};

    // Draw floor - only the TOP face (viewed from above/inside)
    rlSetTexture(floor_tex.id);
    rlBegin(RL_QUADS);
    rlColor4ub(139, 69, 19, 255);  // Brown for floor

    rlTexCoord2f(0.0f, 0.0f);
    rlVertex3f(floor_v0.x, floor_v0.y, floor_v0.z);

    rlTexCoord2f(0.0f, 1.0f);
    rlVertex3f(floor_v3.x, floor_v3.y, floor_v3.z);

    rlTexCoord2f(1.0f, 1.0f);
    rlVertex3f(floor_v2.x, floor_v2.y, floor_v2.z);

    rlTexCoord2f(1.0f, 0.0f);
    rlVertex3f(floor_v1.x, floor_v1.y, floor_v1.z);

    rlEnd();
    rlSetTexture(0);

    // Draw ceiling - only the BOTTOM face (viewed from below/inside)
    rlSetTexture(ceil_tex.id);
    rlBegin(RL_QUADS);
    rlColor4ub(169, 169, 169, 255);  // Gray for ceiling

    rlTexCoord2f(0.0f, 0.0f);
    rlVertex3f(ceil_v0.x, ceil_v0.y, ceil_v0.z);

    rlTexCoord2f(1.0f, 0.0f);
    rlVertex3f(ceil_v1.x, ceil_v1.y, ceil_v1.z);

    rlTexCoord2f(1.0f, 1.0f);
    rlVertex3f(ceil_v2.x, ceil_v2.y, ceil_v2.z);

    rlTexCoord2f(0.0f, 1.0f);
    rlVertex3f(ceil_v3.x, ceil_v3.y, ceil_v3.z);

    rlEnd();
    rlSetTexture(0);
}

void BasicRenderer::draw_textured_quad(const Vector3& v0, const Vector3& v1,
                                        const Vector3& v2, const Vector3& v3,
                                        const Texture2D& texture,
                                        float u_scale, float v_scale) {
    // Manually draw textured quad using rlgl
    rlSetTexture(texture.id);

    rlBegin(RL_QUADS);
    rlColor4ub(255, 255, 255, 255);

    // Bottom-left
    rlTexCoord2f(0.0f, v_scale);
    rlVertex3f(v0.x, v0.y, v0.z);

    // Bottom-right
    rlTexCoord2f(u_scale, v_scale);
    rlVertex3f(v1.x, v1.y, v1.z);

    // Top-right
    rlTexCoord2f(u_scale, 0.0f);
    rlVertex3f(v2.x, v2.y, v2.z);

    // Top-left
    rlTexCoord2f(0.0f, 0.0f);
    rlVertex3f(v3.x, v3.y, v3.z);

    rlEnd();
    rlSetTexture(0);
}

void BasicRenderer::render_sprites(const std::vector<Sprite>& sprites, const game::Camera& camera) {
    if (sprites.empty()) {
        return;
    }

    // Sort sprites by distance from camera (back to front for alpha blending)
    struct SpriteDistance {
        const Sprite* sprite;
        float distance_sq;
    };

    std::vector<SpriteDistance> sorted_sprites;
    sorted_sprites.reserve(sprites.size());

    Vector3 cam_pos = camera.get_position();
    for (const auto& sprite : sprites) {
        float dx = sprite.position.x - cam_pos.x;
        float dy = sprite.position.y - cam_pos.y;
        float dz = sprite.position.z - cam_pos.z;
        float dist_sq = dx * dx + dy * dy + dz * dz;
        sorted_sprites.push_back({&sprite, dist_sq});
    }

    std::sort(sorted_sprites.begin(), sorted_sprites.end(),
             [](const SpriteDistance& a, const SpriteDistance& b) {
                 return a.distance_sq > b.distance_sq;  // Far to near
             });

    // Render sorted sprites
    for (const auto& sd : sorted_sprites) {
        render_sprite(*sd.sprite, camera);
    }
}

void BasicRenderer::render_sprite(const Sprite& sprite, const game::Camera& camera) {
    // Get camera forward and right vectors
    Vector3 cam_pos = camera.get_position();
    Vector3 cam_forward = camera.get_forward();
    Vector3 cam_right = Vector3CrossProduct(cam_forward, {0.0f, 1.0f, 0.0f});
    cam_right = Vector3Normalize(cam_right);

    // Calculate sprite quad corners (billboard facing camera)
    float half_width = sprite.width * 0.5f;
    float anchor_offset = sprite.height * sprite.anchor_y;

    Vector3 bottom_center = sprite.position;
    bottom_center.y += anchor_offset;

    Vector3 top_center = bottom_center;
    top_center.y += sprite.height;

    // Calculate quad corners
    Vector3 bottom_left = Vector3Subtract(bottom_center, Vector3Scale(cam_right, half_width));
    Vector3 bottom_right = Vector3Add(bottom_center, Vector3Scale(cam_right, half_width));
    Vector3 top_left = Vector3Subtract(top_center, Vector3Scale(cam_right, half_width));
    Vector3 top_right = Vector3Add(top_center, Vector3Scale(cam_right, half_width));

    // Get texture
    const Texture2D& texture = m_texture_manager->get_texture(sprite.texture_id);

    // Draw billboard quad
    draw_textured_quad(bottom_left, bottom_right, top_right, top_left,
                      texture, 1.0f, 1.0f);
}

} // namespace rendering
