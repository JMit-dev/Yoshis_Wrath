#include "rendering/animation/animation_controller.h"

namespace rendering {

AnimationController::AnimationController()
    : m_time(0.0f)
    , m_current_frame_index(0)
    , m_playing(false)
    , m_finished(false) {
}

void AnimationController::add_animation(const std::string& name, const Animation& animation) {
    m_animations[name] = animation;
}

void AnimationController::play(const std::string& animation_name, bool restart_if_playing) {
    // Check if animation exists
    auto it = m_animations.find(animation_name);
    if (it == m_animations.end()) {
        return;
    }

    // If already playing this animation and not restarting, continue
    if (m_current_animation == animation_name && m_playing && !restart_if_playing) {
        return;
    }

    // Check if animation is on cooldown
    auto cooldown_it = m_cooldown_timers.find(animation_name);
    if (cooldown_it != m_cooldown_timers.end() && cooldown_it->second > 0.0f) {
        // Animation is on cooldown, can't play yet
        return;
    }

    // Check if current animation can be interrupted
    if (m_playing && !m_current_animation.empty()) {
        auto current_it = m_animations.find(m_current_animation);
        if (current_it != m_animations.end() && !current_it->second.interruptible) {
            // Current animation is non-interruptible, don't switch
            return;
        }
    }

    // Start new animation
    m_current_animation = animation_name;
    m_time = 0.0f;
    m_current_frame_index = 0;
    m_playing = true;
    m_finished = false;
}

void AnimationController::update(float delta_time) {
    // Update cooldown timers
    for (auto& pair : m_cooldown_timers) {
        if (pair.second > 0.0f) {
            pair.second -= delta_time;
            if (pair.second < 0.0f) {
                pair.second = 0.0f;
            }
        }
    }

    if (!m_playing) {
        return;
    }

    auto it = m_animations.find(m_current_animation);
    if (it == m_animations.end()) {
        return;
    }

    const Animation& anim = it->second;

    // Update time
    m_time += delta_time;

    // Get current frame duration (either per-frame or global)
    float current_frame_duration = anim.frame_duration;
    if (anim.use_per_frame_timing && m_current_frame_index < static_cast<int>(anim.per_frame_durations.size())) {
        current_frame_duration = anim.per_frame_durations[m_current_frame_index];
    }

    // Check if we need to advance frame
    if (m_time >= current_frame_duration) {
        m_time -= current_frame_duration;
        m_current_frame_index++;

        // Check if animation finished
        if (m_current_frame_index >= static_cast<int>(anim.frames.size())) {
            if (anim.looping) {
                m_current_frame_index = 0;
            } else {
                // Non-looping animation finished
                m_current_frame_index = static_cast<int>(anim.frames.size()) - 1;
                m_playing = false;
                m_finished = true;

                // Start cooldown timer for this animation
                if (anim.cooldown > 0.0f) {
                    m_cooldown_timers[m_current_animation] = anim.cooldown;
                }
            }
        }
    }
}

int AnimationController::get_current_frame() const {
    auto it = m_animations.find(m_current_animation);
    if (it == m_animations.end()) {
        return 0;
    }

    const Animation& anim = it->second;
    if (m_current_frame_index < 0 || m_current_frame_index >= static_cast<int>(anim.frames.size())) {
        return 0;
    }

    return anim.frames[m_current_frame_index];
}

} // namespace rendering
