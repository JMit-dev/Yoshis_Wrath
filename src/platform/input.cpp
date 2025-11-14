#include "platform/input.h"

namespace platform {

RaylibInputProvider::RaylibInputProvider()
    : m_mouse_sensitivity(1.0f)
    , m_mouse_captured(false)
    , m_last_mouse_position{0.0f, 0.0f} {
}

InputState RaylibInputProvider::get_current_state() const {
    InputState state;

    // Movement keys (WASD)
    state.forward = IsKeyDown(KEY_W);
    state.backward = IsKeyDown(KEY_S);
    state.strafe_left = IsKeyDown(KEY_A);
    state.strafe_right = IsKeyDown(KEY_D);
    state.jump = IsKeyDown(KEY_SPACE);
    state.crouch = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_C);

    // Actions
    state.shoot = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    state.reload = IsKeyPressed(KEY_R);
    state.use = IsKeyPressed(KEY_E);

    // Mouse movement
    if (m_mouse_captured) {
        state.mouse_delta = GetMouseDelta();
        state.mouse_delta.x *= m_mouse_sensitivity;
        state.mouse_delta.y *= m_mouse_sensitivity;
    } else {
        state.mouse_delta = {0.0f, 0.0f};
    }

    state.mouse_sensitivity = m_mouse_sensitivity;

    // Pause
    state.pause = IsKeyPressed(KEY_ESCAPE);

    return state;
}

void RaylibInputProvider::set_mouse_sensitivity(float sensitivity) {
    m_mouse_sensitivity = sensitivity;
}

void RaylibInputProvider::capture_mouse(bool capture) {
    m_mouse_captured = capture;
    if (capture) {
        DisableCursor();
    } else {
        EnableCursor();
    }
}

} // namespace platform
