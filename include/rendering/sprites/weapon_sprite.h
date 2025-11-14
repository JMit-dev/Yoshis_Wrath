#pragma once

#include "rendering/sprites/hud_sprite.h"
#include <memory>
#include <string>

namespace rendering {

// Weapon-specific HUD sprite with attack logic
class WeaponSprite {
public:
    WeaponSprite();
    ~WeaponSprite() = default;

    // Load weapon from JSON definition file
    bool load_from_json(const std::string& json_path);

    // Update animation
    void update(float delta_time);

    // Render weapon to screen (call outside 3D mode)
    void render();

    // Trigger attack animation
    void trigger_attack();

    // Check if attack animation is playing
    bool is_attacking() const;

    // Forward position/scale to underlying HUD sprite
    void set_position(const Vector2& position);
    void set_scale(float scale);

private:
    std::unique_ptr<HUDSprite> m_sprite;
    std::string m_default_animation;
    std::string m_attack_animation;
};

} // namespace rendering
