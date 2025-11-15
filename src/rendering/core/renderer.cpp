#include "rendering/core/renderer.h"
#include "game/bsp.h"
#include "raymath.h"
#include "rlgl.h"
#include <algorithm>

namespace rendering {

BasicRenderer::BasicRenderer()
    : m_texture_manager(std::make_unique<TextureManager>())
    , m_hud(std::make_unique<HUD>())
    , m_sector_renderer(std::make_unique<SectorRenderer>(*m_texture_manager))
    , m_weapon_sprite(std::make_unique<WeaponSprite>())
    , m_render_width(1920)   // Default 1080p resolution
    , m_render_height(1080) {
    // Load weapon sprite
    m_weapon_sprite->load_from_json("sprites/weapon_fist.json");

    // Create initial render target
    m_render_target = LoadRenderTexture(m_render_width, m_render_height);

    // Set initial UI scaling
    update_ui_scaling();
}

void BasicRenderer::update_render_target() {
    // No need to recreate render target - it stays at game resolution
    // Window can be any size and we'll letterbox/pillarbox as needed
}

void BasicRenderer::update_ui_scaling() {
    // Position weapon at bottom center, scaled relative to render size
    float weapon_x = m_render_width / 2.0f;
    float weapon_y = m_render_height - 64.0f;  // 64 pixels from bottom
    m_weapon_sprite->set_position({weapon_x, weapon_y});

    // Scale weapon relative to render height (2.0 for 1080p looks good)
    float weapon_scale = m_render_height / 540.0f;
    m_weapon_sprite->set_scale(weapon_scale);
}

void BasicRenderer::begin_frame() {
    // Begin drawing to render target
    BeginTextureMode(m_render_target);
    ClearBackground(BLACK);
}

void BasicRenderer::end_frame() {
    // End drawing to render target
    EndTextureMode();

    // Now draw the render target to the window with letterboxing
    BeginDrawing();
    ClearBackground(BLACK);

    int window_width = GetScreenWidth();
    int window_height = GetScreenHeight();

    // Calculate scaling to fit render target in window while preserving aspect ratio
    float target_aspect = static_cast<float>(m_render_width) / static_cast<float>(m_render_height);
    float window_aspect = static_cast<float>(window_width) / static_cast<float>(window_height);

    int draw_width, draw_height, x_offset, y_offset;

    if (window_aspect > target_aspect) {
        // Window is wider than target - pillarbox (black bars on sides)
        draw_height = window_height;
        draw_width = static_cast<int>(window_height * target_aspect);
        x_offset = (window_width - draw_width) / 2;
        y_offset = 0;
    } else {
        // Window is taller than target - letterbox (black bars on top/bottom)
        draw_width = window_width;
        draw_height = static_cast<int>(window_width / target_aspect);
        x_offset = 0;
        y_offset = (window_height - draw_height) / 2;
    }

    // Draw the render target (flip vertically since render textures are upside down)
    Rectangle source = {0, 0, static_cast<float>(m_render_target.texture.width),
                       static_cast<float>(-m_render_target.texture.height)};
    Rectangle dest = {static_cast<float>(x_offset), static_cast<float>(y_offset),
                      static_cast<float>(draw_width), static_cast<float>(draw_height)};

    DrawTexturePro(m_render_target.texture, source, dest, {0, 0}, 0.0f, WHITE);

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
            m_sector_renderer->render_sector(sectors[idx]);
        }
    }

    EndMode3D();

    // Draw HUD and weapon
    m_hud->render();
    m_weapon_sprite->render();
}

void BasicRenderer::trigger_weapon_attack() {
    m_weapon_sprite->trigger_attack();
}

void BasicRenderer::update_weapon(float delta_time) {
    m_weapon_sprite->update(delta_time);
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
