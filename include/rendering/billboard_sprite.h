#pragma once

#include "rendering/base_sprite.h"
#include "game/camera.h"
#include "raylib.h"

namespace rendering {

// 3D world-space billboard sprite (enemies, pickups, effects)
// Always faces the camera
class BillboardSprite : public BaseSprite {
public:
    BillboardSprite();
    ~BillboardSprite() override = default;

    // Load sprite from JSON definition
    bool load_from_json(const std::string& json_path);

    // Render sprite in 3D world space (must be called inside 3D mode)
    void render() override;

    // Render with explicit camera (for sorting/culling)
    void render_with_camera(const game::Camera& camera);

    // Position and size
    void set_position(const Vector3& position);
    void set_size(float width, float height);
    void set_anchor_y(float anchor);  // 0.0 = bottom, 0.5 = center, 1.0 = top

    const Vector3& get_position() const { return m_position; }
    float get_width() const { return m_width; }
    float get_height() const { return m_height; }

    // For depth sorting
    float get_distance_sq_from(const Vector3& point) const;

private:
    Vector3 m_position;
    float m_width;
    float m_height;
    float m_anchor_y;
};

} // namespace rendering
