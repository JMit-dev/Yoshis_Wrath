#include "rendering/scene/wall_renderer.h"
#include "rlgl.h"
#include <cmath>

namespace rendering {

WallRenderer::WallRenderer(TextureManager& texture_manager)
    : m_texture_manager(texture_manager) {
}

void WallRenderer::render_wall(const game::Sector& sector, const game::Wall& wall) {
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
    const Texture2D& texture = m_texture_manager.get_texture(wall.texture_id);

    // Calculate wall length for UV scaling
    float wall_length = sqrtf(
        (v2.x - v1.x) * (v2.x - v1.x) +
        (v2.z - v1.z) * (v2.z - v1.z)
    );
    float wall_height = sector.ceiling_height - sector.floor_height;

    // Draw textured quad
    draw_textured_quad(bottom_left, bottom_right, top_right, top_left,
                      texture, wall_length, wall_height);
}

void WallRenderer::draw_textured_quad(const Vector3& v0, const Vector3& v1,
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

} // namespace rendering
