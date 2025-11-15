#pragma once

#include "game/level.h"
#include "game/camera.h"
#include "rendering/textures/texture_manager.h"
#include "rendering/sprites/sprite.h"
#include "rendering/core/hud.h"
#include "rendering/scene/sector_renderer.h"
#include "rendering/sprites/weapon_sprite.h"
#include "raylib.h"
#include <memory>
#include <vector>

namespace rendering {

// Pure rendering interface - no game logic
class IRenderer {
public:
    virtual ~IRenderer() = default;

    // Render a frame
    virtual void render(const game::Level& level, const game::Camera& camera) = 0;
    virtual void render_sprites(const std::vector<Sprite>& sprites, const game::Camera& camera) = 0;

    // Clear screen
    virtual void begin_frame() = 0;
    virtual void end_frame() = 0;
};

// Basic 3D renderer using Raylib with BSP traversal and textures
class BasicRenderer : public IRenderer {
public:
    BasicRenderer();
    ~BasicRenderer() override = default;

    void render(const game::Level& level, const game::Camera& camera) override;
    void render_sprites(const std::vector<Sprite>& sprites, const game::Camera& camera) override;
    void begin_frame() override;
    void end_frame() override;

    // Weapon controls
    void trigger_weapon_attack();
    void update_weapon(float delta_time);

private:
    std::unique_ptr<TextureManager> m_texture_manager;
    std::unique_ptr<HUD> m_hud;
    std::unique_ptr<SectorRenderer> m_sector_renderer;
    std::unique_ptr<WeaponSprite> m_weapon_sprite;

    RenderTexture2D m_render_target;  // Fixed resolution render target
    int m_render_width;                // Target render width
    int m_render_height;               // Target render height

    void render_sprite(const Sprite& sprite, const game::Camera& camera);
    void update_render_target();       // Update render target on window resize
    void update_ui_scaling();          // Update UI element positions based on render size

    // Helper to draw a textured quad for sprites
    void draw_textured_quad(const Vector3& v0, const Vector3& v1,
                           const Vector3& v2, const Vector3& v3,
                           const Texture2D& texture,
                           float u_scale = 1.0f, float v_scale = 1.0f);
};

} // namespace rendering
