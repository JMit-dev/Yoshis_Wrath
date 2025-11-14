#include "game/game_state.h"

namespace game {

GameState::GameState()
    : m_is_paused(false) {
}

void GameState::initialize(const Level& level) {
    m_level = level;

    // Set camera to player spawn point if available
    const auto& spawns = m_level.get_spawns();
    if (!spawns.empty()) {
        m_camera.set_position(spawns[0].position);
    }
}

void GameState::update(float delta_time, const platform::InputState& input) {
    if (m_is_paused) {
        return;
    }

    // Update camera
    m_camera.update(input, delta_time);

    // TODO: Update entities
    // TODO: Handle collisions
    // TODO: Process game logic
}

} // namespace game
