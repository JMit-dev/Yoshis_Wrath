#pragma once

#include "raylib.h"
#include "platform/input.h"

namespace game {

// First-person camera for FPS-style movement
class Camera {
public:
    Camera();
    Camera(Vector3 position, Vector3 target, float fov);
    ~Camera() = default;

    // Update camera based on input state
    void update(const platform::InputState& input, float delta_time);

    // Getters
    const Vector3& get_position() const { return m_position; }
    const Vector3& get_target() const { return m_target; }
    const Vector3& get_up() const { return m_up; }
    float get_fov() const { return m_fov; }
    float get_yaw() const { return m_yaw; }
    float get_pitch() const { return m_pitch; }

    // Get forward direction vector (normalized)
    Vector3 get_forward() const;

    // Setters
    void set_position(const Vector3& position);
    void set_target(const Vector3& target);
    void set_fov(float fov);
    void set_movement_speed(float speed);
    void set_mouse_sensitivity(float sensitivity);

    // Get Raylib Camera3D for rendering
    Camera3D to_raylib_camera() const;

private:
    Vector3 m_position;
    Vector3 m_target;
    Vector3 m_up;

    float m_fov;
    float m_yaw;         // Horizontal rotation (radians)
    float m_pitch;       // Vertical rotation (radians)

    float m_movement_speed;
    float m_mouse_sensitivity;

    void update_target_from_angles();
    void clamp_pitch();
};

} // namespace game
