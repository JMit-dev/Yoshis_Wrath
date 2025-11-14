#pragma once

#include "rendering/texture_atlas.h"
#include "rendering/animation_controller.h"
#include "raylib.h"
#include <memory>
#include <string>

namespace rendering {

// HUD weapon sprite with animation support
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

    // Set screen position (centered by default)
    void set_position(const Vector2& position);

    // Set scale
    void set_scale(float scale);

private:
    std::unique_ptr<TextureAtlas> m_atlas;
    AnimationController m_animation_controller;

    Vector2 m_position;
    float m_scale;

    std::string m_default_animation;
    std::string m_attack_animation;
};

} // namespace rendering
