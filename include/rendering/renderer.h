#pragma once

#include "game/level.h"
#include "game/camera.h"
#include "rendering/texture_manager.h"
#include "raylib.h"
#include <memory>

namespace rendering {

// Pure rendering interface - no game logic
class IRenderer {
public:
    virtual ~IRenderer() = default;

    // Render a frame
    virtual void render(const game::Level& level, const game::Camera& camera) = 0;

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
    void begin_frame() override;
    void end_frame() override;

private:
    std::unique_ptr<TextureManager> m_texture_manager;

    void render_sector(const game::Sector& sector);
    void render_wall(const game::Sector& sector, const game::Wall& wall);
    void render_floor_ceiling(const game::Sector& sector);

    // Helper to draw a textured quad
    void draw_textured_quad(const Vector3& v0, const Vector3& v1,
                           const Vector3& v2, const Vector3& v3,
                           const Texture2D& texture,
                           float u_scale = 1.0f, float v_scale = 1.0f);
};

} // namespace rendering
