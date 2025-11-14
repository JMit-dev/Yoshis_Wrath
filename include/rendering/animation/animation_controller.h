#pragma once

#include <string>
#include <vector>
#include <map>

namespace rendering {

// Represents a single animation sequence
struct Animation {
    std::string name;
    std::vector<int> frames;     // Frame indices in the atlas
    float frame_duration;         // Duration per frame in seconds
    bool looping;

    Animation()
        : frame_duration(0.1f)
        , looping(true) {
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
    std::string m_current_animation;
    float m_time;
    int m_current_frame_index;
    bool m_playing;
    bool m_finished;
};

} // namespace rendering
