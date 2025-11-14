#pragma once

#include "game/level.h"
#include "rendering/wall_renderer.h"
#include "rendering/floor_ceiling_renderer.h"
#include <memory>

namespace rendering {

// Coordinates rendering of a complete sector (walls + floor + ceiling)
class SectorRenderer {
public:
    SectorRenderer(TextureManager& texture_manager);
    ~SectorRenderer() = default;

    // Render a complete sector
    void render_sector(const game::Sector& sector);

private:
    std::unique_ptr<WallRenderer> m_wall_renderer;
    std::unique_ptr<FloorCeilingRenderer> m_floor_ceiling_renderer;
};

} // namespace rendering
