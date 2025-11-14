#pragma once

#include "rendering/texture_atlas.h"
#include "rendering/animation_controller.h"
#include "raylib.h"
#include <memory>

namespace rendering {

// Abstract base class for all sprite types
class BaseSprite {
public:
    virtual ~BaseSprite() = default;

    // Update animation state
    virtual void update(float delta_time);

    // Render the sprite (implementation depends on sprite type)
    virtual void render() = 0;

    // Animation control
    void play_animation(const std::string& animation_name, bool restart = false);
    bool is_animation_playing(const std::string& animation_name) const;
    bool is_animation_finished() const;

    // Getters
    const TextureAtlas* get_atlas() const { return m_atlas.get(); }
    const AnimationController& get_animation_controller() const { return m_animation_controller; }
    AnimationController& get_animation_controller() { return m_animation_controller; }

protected:
    BaseSprite();

    std::unique_ptr<TextureAtlas> m_atlas;
    AnimationController m_animation_controller;
};

} // namespace rendering
