#pragma once

#include "game/level.h"
#include "game/camera.h"
#include "platform/input.h"
#include <memory>

namespace game {

// Pure game state - no rendering, no input handling
// Just game logic and data
class GameState {
public:
    GameState();
    ~GameState() = default;

    // Initialize game with a level (takes ownership via move)
    void initialize(Level&& level);

    // Update game state
    void update(float delta_time, const platform::InputState& input);

    // Getters for rendering
    const Level& get_level() const { return m_level; }
    const Camera& get_camera() const { return m_camera; }

    // Game state
    bool is_paused() const { return m_is_paused; }
    void set_paused(bool paused) { m_is_paused = paused; }

private:
    Level m_level;
    Camera m_camera;
    bool m_is_paused;
};

} // namespace game
