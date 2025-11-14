#pragma once

#include <string>
#include <vector>
#include <map>

namespace rendering {

// Represents a single animation sequence
struct Animation {
    std::string name;
    std::vector<int> frames;           // Frame indices in the atlas
    float frame_duration;               // Default duration per frame in seconds (if not using per_frame_durations)
    std::vector<float> per_frame_durations;  // Optional: individual duration for each frame
    bool use_per_frame_timing;          // If true, use per_frame_durations instead of frame_duration
    bool looping;
    bool interruptible;                 // Can this animation be interrupted by another?
    float cooldown;                     // Time in seconds before animation can be played again

    Animation()
        : frame_duration(0.1f)
        , use_per_frame_timing(false)
        , looping(true)
        , interruptible(true)
        , cooldown(0.0f) {
    }
};

// Controls animation playback for sprites
class AnimationController {
public:
    AnimationController();
    ~AnimationController() = default;

    // Add an animation definition
    void add_animation(const std::string& name, const Animation& animation);

    // Play a specific animation
    void play(const std::string& animation_name, bool restart_if_playing = false);

    // Update animation state
    void update(float delta_time);

    // Get current frame index
    int get_current_frame() const;

    // Check if animation is playing
    bool is_playing() const { return m_playing; }

    // Check if current animation has finished (for non-looping animations)
    bool is_finished() const { return m_finished; }

    // Get current animation name
    const std::string& get_current_animation() const { return m_current_animation; }

private:
    std::map<std::string, Animation> m_animations;
    std::map<std::string, float> m_cooldown_timers;  // Tracks remaining cooldown time per animation
    std::string m_current_animation;
    float m_time;
    int m_current_frame_index;
    bool m_playing;
    bool m_finished;
};

} // namespace rendering
