#include "rendering/sprites/weapon_sprite.h"
#include "platform/file_system.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

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

    // Load and parse JSON file
    std::string full_path = platform::FileSystem::join_path(
        platform::FileSystem::get_assets_path(), json_path);
    std::ifstream file(full_path);
    if (!file.is_open()) {
        return false;
    }

    json j;
    try {
        file >> j;
    } catch (const std::exception&) {
        return false;
    }
    file.close();

    // Parse animations from JSON
    if (!j.contains("animations") || !j.contains("default_animation")) {
        return false;
    }

    const auto& animations = j["animations"];
    for (auto it = animations.begin(); it != animations.end(); ++it) {
        Animation anim;
        anim.name = it.key();

        const auto& anim_data = it.value();

        // Parse frames array
        if (anim_data.contains("frames") && anim_data["frames"].is_array()) {
            anim.frames = anim_data["frames"].get<std::vector<int>>();
        }

        // Parse timing data
        anim.use_per_frame_timing = anim_data.value("use_per_frame_timing", false);

        if (anim.use_per_frame_timing && anim_data.contains("per_frame_durations")) {
            anim.per_frame_durations = anim_data["per_frame_durations"].get<std::vector<float>>();
        } else {
            anim.frame_duration = anim_data.value("frame_duration", 0.1f);
        }

        // Parse animation properties
        anim.looping = anim_data.value("looping", true);
        anim.interruptible = anim_data.value("interruptible", true);
        anim.cooldown = anim_data.value("cooldown", 0.0f);

        m_sprite->get_animation_controller().add_animation(anim.name, anim);
    }

    // Start with default animation
    std::string default_anim = j["default_animation"];
    m_sprite->play_animation(default_anim);
    m_default_animation = default_anim;

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
