#include "rendering/renderer.h"
#include "raymath.h"

namespace rendering {

BasicRenderer::BasicRenderer() {
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

    // Render all sectors (simple approach for now, BSP traversal will come later)
    const auto& sectors = level.get_sectors();
    for (const auto& sector : sectors) {
        render_sector(sector);
    }

    // Draw a grid for reference
    DrawGrid(20, 1.0f);

    EndMode3D();

    // Draw HUD
    DrawText("Yoshi's Wrath - Engine Test", 10, 10, 20, GREEN);
    DrawText("WASD: Move | Mouse: Look | ESC: Exit", 10, 40, 16, LIGHTGRAY);
    DrawFPS(10, GetScreenHeight() - 30);
}

void BasicRenderer::render_sector(const game::Sector& sector) {
    render_floor_ceiling(sector);

    // Render walls
    for (const auto& wall : sector.walls) {
        render_wall(sector, wall);
    }
}

void BasicRenderer::render_wall(const game::Sector& sector, const game::Wall& wall) {
    const game::Vertex& v1 = sector.vertices[wall.vertex_a];
    const game::Vertex& v2 = sector.vertices[wall.vertex_b];

    // Wall corners
    Vector3 bottom_left = {v1.x, sector.floor_height, v1.z};
    Vector3 bottom_right = {v2.x, sector.floor_height, v2.z};
    Vector3 top_left = {v1.x, sector.ceiling_height, v1.z};
    Vector3 top_right = {v2.x, sector.ceiling_height, v2.z};

    // Choose color based on whether it's a portal
    Color wall_color = (wall.portal_id >= 0) ? BLUE : GRAY;

    // Draw wall as two triangles
    DrawTriangle3D(bottom_left, top_left, top_right, wall_color);
    DrawTriangle3D(bottom_left, top_right, bottom_right, wall_color);

    // Draw wireframe outline
    DrawLine3D(bottom_left, bottom_right, DARKGRAY);
    DrawLine3D(bottom_left, top_left, DARKGRAY);
    DrawLine3D(top_left, top_right, DARKGRAY);
    DrawLine3D(bottom_right, top_right, DARKGRAY);
}

void BasicRenderer::render_floor_ceiling(const game::Sector& sector) {
    // Simple triangulation of sector polygon (assumes convex sector)
    if (sector.vertices.size() < 3) {
        return;
    }

    const game::Vertex& center = sector.vertices[0];

    for (size_t i = 1; i < sector.vertices.size() - 1; ++i) {
        const game::Vertex& v1 = sector.vertices[i];
        const game::Vertex& v2 = sector.vertices[i + 1];

        // Floor triangle
        Vector3 floor_p0 = {center.x, sector.floor_height, center.z};
        Vector3 floor_p1 = {v1.x, sector.floor_height, v1.z};
        Vector3 floor_p2 = {v2.x, sector.floor_height, v2.z};
        DrawTriangle3D(floor_p0, floor_p1, floor_p2, DARKBROWN);

        // Ceiling triangle
        Vector3 ceil_p0 = {center.x, sector.ceiling_height, center.z};
        Vector3 ceil_p1 = {v1.x, sector.ceiling_height, v1.z};
        Vector3 ceil_p2 = {v2.x, sector.ceiling_height, v2.z};
        DrawTriangle3D(ceil_p2, ceil_p1, ceil_p0, DARKGRAY);  // Reversed for proper culling
    }
}

} // namespace rendering
