#include "rendering/texture_atlas.h"
#include "raylib.h"

namespace rendering {

TextureAtlas::TextureAtlas()
    : m_texture{0}
    , m_frame_width(0)
    , m_frame_height(0)
    , m_frames_horizontal(0)
    , m_frames_vertical(0)
    , m_loaded(false) {
}

TextureAtlas::~TextureAtlas() {
    if (m_loaded && m_texture.id != 0) {
        UnloadTexture(m_texture);
    }
}

bool TextureAtlas::load(const std::string& path, int frame_width, int frame_height,
                        int frames_horizontal, int frames_vertical) {
    // Load texture from file
    m_texture = LoadTexture((std::string(ASSETS_PATH) + path).c_str());

    if (m_texture.id == 0) {
        return false;
    }

    m_frame_width = frame_width;
    m_frame_height = frame_height;
    m_frames_horizontal = frames_horizontal;
    m_frames_vertical = frames_vertical;

    // Calculate frame UV rectangles
    m_frames.clear();
    for (int y = 0; y < frames_vertical; ++y) {
        for (int x = 0; x < frames_horizontal; ++x) {
            Rectangle frame_rect;
            frame_rect.x = static_cast<float>(x * frame_width);
            frame_rect.y = static_cast<float>(y * frame_height);
            frame_rect.width = static_cast<float>(frame_width);
            frame_rect.height = static_cast<float>(frame_height);
            m_frames.push_back(frame_rect);
        }
    }

    m_loaded = true;
    return true;
}

Rectangle TextureAtlas::get_frame_uv(int frame_index) const {
    if (frame_index < 0 || frame_index >= static_cast<int>(m_frames.size())) {
        return m_frames[0];  // Return first frame as fallback
    }
    return m_frames[frame_index];
}

} // namespace rendering
