#pragma once

#include "raylib.h"

namespace platform {

// Input state capturing keyboard and mouse state
struct InputState {
    // Movement keys
    bool forward;
    bool backward;
    bool strafe_left;
    bool strafe_right;
    bool jump;
    bool crouch;

    // Actions
    bool shoot;
    bool reload;
    bool use;

    // Mouse
    Vector2 mouse_delta;
    float mouse_sensitivity;

    // Misc
    bool pause;

    InputState()
        : forward(false)
        , backward(false)
        , strafe_left(false)
        , strafe_right(false)
        , jump(false)
        , crouch(false)
        , shoot(false)
        , reload(false)
        , use(false)
        , mouse_delta{0.0f, 0.0f}
        , mouse_sensitivity(1.0f)
        , pause(false) {}
};

// Platform input provider interface
class IInputProvider {
public:
    virtual ~IInputProvider() = default;
    virtual InputState get_current_state() const = 0;
};

// Raylib-based input implementation
class RaylibInputProvider : public IInputProvider {
public:
    RaylibInputProvider();
    ~RaylibInputProvider() override = default;

    InputState get_current_state() const override;

    void set_mouse_sensitivity(float sensitivity);
    void capture_mouse(bool capture);

private:
    float m_mouse_sensitivity;
    bool m_mouse_captured;
    Vector2 m_last_mouse_position;
};

} // namespace platform
