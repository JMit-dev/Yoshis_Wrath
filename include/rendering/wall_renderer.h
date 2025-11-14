#pragma once

#include "raylib.h"
#include "game/level.h"
#include "rendering/texture_manager.h"

namespace rendering {

// Handles rendering of individual walls with proper UV mapping
class WallRenderer {
public:
    WallRenderer(TextureManager& texture_manager);
    ~WallRenderer() = default;

    // Render a single wall from a sector
    void render_wall(const game::Sector& sector, const game::Wall& wall);

private:
    TextureManager& m_texture_manager;

    // Helper to draw a textured quad
    void draw_textured_quad(const Vector3& v0, const Vector3& v1,
                           const Vector3& v2, const Vector3& v3,
                           const Texture2D& texture,
                           float u_scale = 1.0f, float v_scale = 1.0f);
};

} // namespace rendering
