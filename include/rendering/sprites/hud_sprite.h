#pragma once

#include "rendering/sprites/base_sprite.h"
#include "raylib.h"
#include <string>

namespace rendering {

// 2D screen-space sprite (weapons, UI elements)
class HUDSprite : public BaseSprite {
public:
    HUDSprite();
    ~HUDSprite() override = default;

    // Load sprite from JSON definition
    bool load_from_json(const std::string& json_path);

    // Render sprite to screen (2D overlay)
    void render() override;

    // Position and scale
    void set_position(const Vector2& position);
    void set_scale(float scale);

    const Vector2& get_position() const { return m_position; }
    float get_scale() const { return m_scale; }

private:
    Vector2 m_position;
    float m_scale;
};

} // namespace rendering
