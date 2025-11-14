#include "game/camera.h"
#include "raymath.h"
#include <cmath>

namespace game {

Camera::Camera()
    : m_position{0.0f, 1.7f, 0.0f}  // Average eye height
    , m_target{0.0f, 1.7f, 1.0f}
    , m_up{0.0f, 1.0f, 0.0f}
    , m_fov(90.0f)
    , m_yaw(0.0f)
    , m_pitch(0.0f)
    , m_movement_speed(5.0f)
    , m_mouse_sensitivity(0.003f) {
}

Camera::Camera(Vector3 position, Vector3 target, float fov)
    : m_position(position)
    , m_target(target)
    , m_up{0.0f, 1.0f, 0.0f}
    , m_fov(fov)
    , m_yaw(0.0f)
    , m_pitch(0.0f)
    , m_movement_speed(5.0f)
    , m_mouse_sensitivity(0.003f) {

    // Calculate initial yaw and pitch from target
    Vector3 direction = Vector3Normalize(Vector3Subtract(target, position));
    m_yaw = atan2f(direction.x, direction.z);
    m_pitch = asinf(direction.y);
}

void Camera::update(const platform::InputState& input, float delta_time) {
    // Update rotation from mouse
    m_yaw -= input.mouse_delta.x * m_mouse_sensitivity;
    m_pitch -= input.mouse_delta.y * m_mouse_sensitivity;

    // Clamp pitch to prevent camera flipping
    clamp_pitch();

    // Update target based on angles
    update_target_from_angles();

    // Calculate movement vectors
    Vector3 forward = Vector3Normalize(Vector3Subtract(m_target, m_position));
    forward.y = 0.0f;  // Keep movement on horizontal plane
    forward = Vector3Normalize(forward);

    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, m_up));

    // Accumulate movement direction
    Vector3 movement_dir = {0.0f, 0.0f, 0.0f};

    if (input.forward) {
        movement_dir = Vector3Add(movement_dir, forward);
    }
    if (input.backward) {
        movement_dir = Vector3Subtract(movement_dir, forward);
    }
    if (input.strafe_right) {
        movement_dir = Vector3Add(movement_dir, right);
    }
    if (input.strafe_left) {
        movement_dir = Vector3Subtract(movement_dir, right);
    }

    // Normalize to prevent faster diagonal movement
    float movement_length = Vector3Length(movement_dir);
    if (movement_length > 0.0f) {
        movement_dir = Vector3Normalize(movement_dir);
        float move_speed = m_movement_speed * delta_time;
        m_position = Vector3Add(m_position, Vector3Scale(movement_dir, move_speed));
    }

    // Update target to follow position
    update_target_from_angles();
}

void Camera::set_position(const Vector3& position) {
    m_position = position;
    update_target_from_angles();
}

void Camera::set_target(const Vector3& target) {
    m_target = target;
    // Recalculate angles from new target
    Vector3 direction = Vector3Normalize(Vector3Subtract(target, m_position));
    m_yaw = atan2f(direction.x, direction.z);
    m_pitch = asinf(direction.y);
    clamp_pitch();
}

void Camera::set_fov(float fov) {
    m_fov = fov;
}

void Camera::set_movement_speed(float speed) {
    m_movement_speed = speed;
}

void Camera::set_mouse_sensitivity(float sensitivity) {
    m_mouse_sensitivity = sensitivity;
}

Camera3D Camera::to_raylib_camera() const {
    Camera3D camera = {0};
    camera.position = m_position;
    camera.target = m_target;
    camera.up = m_up;
    camera.fovy = m_fov;
    camera.projection = CAMERA_PERSPECTIVE;
    return camera;
}

void Camera::update_target_from_angles() {
    // Calculate direction vector from yaw and pitch
    Vector3 direction;
    direction.x = sinf(m_yaw) * cosf(m_pitch);
    direction.y = sinf(m_pitch);
    direction.z = cosf(m_yaw) * cosf(m_pitch);

    m_target = Vector3Add(m_position, direction);
}

void Camera::clamp_pitch() {
    const float max_pitch = 1.5f;  // ~85 degrees
    if (m_pitch > max_pitch) {
        m_pitch = max_pitch;
    }
    if (m_pitch < -max_pitch) {
        m_pitch = -max_pitch;
    }
}

} // namespace game
