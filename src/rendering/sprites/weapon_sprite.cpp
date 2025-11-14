#include "rendering/sprites/weapon_sprite.h"

namespace rendering {

WeaponSprite::WeaponSprite()
    : m_sprite(std::make_unique<HUDSprite>())
    , m_default_animation("idle")
    , m_attack_animation("punch") {
}

bool WeaponSprite::load_from_json(const std::string& json_path) {
    // Load sprite using HUDSprite
    if (!m_sprite->load_from_json(json_path)) {
        return false;
    }

    // Add animations (hardcoded for now, we can extend this later)
    Animation idle_anim;
    idle_anim.name = "idle";
    idle_anim.frames = {0};
    idle_anim.frame_duration = 0.1f;
    idle_anim.looping = true;
    idle_anim.interruptible = true;
    m_sprite->get_animation_controller().add_animation("idle", idle_anim);

    Animation punch_anim;
    punch_anim.name = "punch";
    punch_anim.frames = {0, 1, 2, 3, 2, 1, 0};
    punch_anim.frame_duration = 0.05f;
    punch_anim.looping = false;
    punch_anim.interruptible = false;  // Cannot interrupt punch animation
    m_sprite->get_animation_controller().add_animation("punch", punch_anim);

    // Start with idle animation
    m_sprite->play_animation("idle");

    return true;
}

void WeaponSprite::update(float delta_time) {
    m_sprite->update(delta_time);

    // If attack animation finished, return to idle
    if (m_sprite->is_animation_finished()) {
        m_sprite->play_animation("idle");
    }
}

void WeaponSprite::render() {
    m_sprite->render();
}

void WeaponSprite::trigger_attack() {
    m_sprite->play_animation("punch", true);
}

bool WeaponSprite::is_attacking() const {
    return m_sprite->is_animation_playing("punch");
}

void WeaponSprite::set_position(const Vector2& position) {
    m_sprite->set_position(position);
}

void WeaponSprite::set_scale(float scale) {
    m_sprite->set_scale(scale);
}

} // namespace rendering
