#pragma once

#include "game/level.h"
#include "game/camera.h"
#include "rendering/texture_manager.h"
#include "rendering/sprite.h"
#include "rendering/hud.h"
#include "rendering/sector_renderer.h"
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

private:
    std::unique_ptr<TextureManager> m_texture_manager;
    std::unique_ptr<HUD> m_hud;
    std::unique_ptr<SectorRenderer> m_sector_renderer;

    void render_sprite(const Sprite& sprite, const game::Camera& camera);

    // Helper to draw a textured quad for sprites
    void draw_textured_quad(const Vector3& v0, const Vector3& v1,
                           const Vector3& v2, const Vector3& v3,
                           const Texture2D& texture,
                           float u_scale = 1.0f, float v_scale = 1.0f);
};

} // namespace rendering
