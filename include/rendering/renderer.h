#pragma once

#include "game/level.h"
#include "game/camera.h"
#include "raylib.h"

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

// Basic 3D renderer using Raylib (placeholder for BSP renderer)
class BasicRenderer : public IRenderer {
public:
    BasicRenderer();
    ~BasicRenderer() override = default;

    void render(const game::Level& level, const game::Camera& camera) override;
    void begin_frame() override;
    void end_frame() override;

private:
    void render_sector(const game::Sector& sector);
    void render_wall(const game::Sector& sector, const game::Wall& wall);
    void render_floor_ceiling(const game::Sector& sector);
};

} // namespace rendering
