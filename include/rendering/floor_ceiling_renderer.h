#pragma once

#include "raylib.h"
#include "game/level.h"
#include "rendering/texture_manager.h"

namespace rendering {

// Handles rendering of floor and ceiling surfaces
class FloorCeilingRenderer {
public:
    FloorCeilingRenderer(TextureManager& texture_manager);
    ~FloorCeilingRenderer() = default;

    // Render floor and ceiling for a sector
    void render_floor_ceiling(const game::Sector& sector);

private:
    TextureManager& m_texture_manager;

    // Render a horizontal quad (floor or ceiling)
    void render_horizontal_quad(const Vector3& v0, const Vector3& v1,
                               const Vector3& v2, const Vector3& v3,
                               const Texture2D& texture,
                               const Color& tint,
                               bool flip_winding);
};

} // namespace rendering
