#include "rendering/floor_ceiling_renderer.h"
#include "rlgl.h"

namespace rendering {

FloorCeilingRenderer::FloorCeilingRenderer(TextureManager& texture_manager)
    : m_texture_manager(texture_manager) {
}

void FloorCeilingRenderer::render_floor_ceiling(const game::Sector& sector) {
    // For now, render as a simple quad (assumes rectangular room)
    if (sector.vertices.size() != 4) {
        return;  // Skip non-rectangular sectors for now
    }

    // Get textures
    const Texture2D& floor_tex = m_texture_manager.get_texture(sector.floor_texture);
    const Texture2D& ceil_tex = m_texture_manager.get_texture(sector.ceiling_texture);

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

    // Render floor (brown tint)
    Color floor_color = {139, 69, 19, 255};
    render_horizontal_quad(floor_v0, floor_v1, floor_v2, floor_v3,
                          floor_tex, floor_color, false);

    // Render ceiling (gray tint)
    Color ceil_color = {169, 169, 169, 255};
    render_horizontal_quad(ceil_v0, ceil_v1, ceil_v2, ceil_v3,
                          ceil_tex, ceil_color, true);
}

void FloorCeilingRenderer::render_horizontal_quad(const Vector3& v0, const Vector3& v1,
                                                   const Vector3& v2, const Vector3& v3,
                                                   const Texture2D& texture,
                                                   const Color& tint,
                                                   bool flip_winding) {
    rlSetTexture(texture.id);
    rlBegin(RL_QUADS);
    rlColor4ub(tint.r, tint.g, tint.b, tint.a);

    if (flip_winding) {
        // Ceiling: viewed from below (clockwise winding)
        rlTexCoord2f(0.0f, 0.0f);
        rlVertex3f(v0.x, v0.y, v0.z);

        rlTexCoord2f(1.0f, 0.0f);
        rlVertex3f(v1.x, v1.y, v1.z);

        rlTexCoord2f(1.0f, 1.0f);
        rlVertex3f(v2.x, v2.y, v2.z);

        rlTexCoord2f(0.0f, 1.0f);
        rlVertex3f(v3.x, v3.y, v3.z);
    } else {
        // Floor: viewed from above (counter-clockwise winding)
        rlTexCoord2f(0.0f, 0.0f);
        rlVertex3f(v0.x, v0.y, v0.z);

        rlTexCoord2f(0.0f, 1.0f);
        rlVertex3f(v3.x, v3.y, v3.z);

        rlTexCoord2f(1.0f, 1.0f);
        rlVertex3f(v2.x, v2.y, v2.z);

        rlTexCoord2f(1.0f, 0.0f);
        rlVertex3f(v1.x, v1.y, v1.z);
    }

    rlEnd();
    rlSetTexture(0);
}

} // namespace rendering
