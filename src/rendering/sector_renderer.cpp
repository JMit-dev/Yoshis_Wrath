#include "rendering/sector_renderer.h"

namespace rendering {

SectorRenderer::SectorRenderer(TextureManager& texture_manager)
    : m_wall_renderer(std::make_unique<WallRenderer>(texture_manager))
    , m_floor_ceiling_renderer(std::make_unique<FloorCeilingRenderer>(texture_manager)) {
}

void SectorRenderer::render_sector(const game::Sector& sector) {
    // Render walls first
    for (const auto& wall : sector.walls) {
        m_wall_renderer->render_wall(sector, wall);
    }

    // Then floor/ceiling
    m_floor_ceiling_renderer->render_floor_ceiling(sector);
}

} // namespace rendering
