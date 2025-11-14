#include "rendering/sprites/base_sprite.h"

namespace rendering {

BaseSprite::BaseSprite()
    : m_atlas(std::make_unique<TextureAtlas>()) {
}

void BaseSprite::update(float delta_time) {
    m_animation_controller.update(delta_time);
}

void BaseSprite::play_animation(const std::string& animation_name, bool restart) {
    m_animation_controller.play(animation_name, restart);
}

bool BaseSprite::is_animation_playing(const std::string& animation_name) const {
    return m_animation_controller.get_current_animation() == animation_name &&
           m_animation_controller.is_playing();
}

bool BaseSprite::is_animation_finished() const {
    return m_animation_controller.is_finished();
}

} // namespace rendering
