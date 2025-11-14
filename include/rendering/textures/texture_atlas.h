#pragma once

#include "raylib.h"
#include <vector>
#include <string>

namespace rendering {

// Represents a texture atlas (sprite sheet) with multiple frames
class TextureAtlas {
public:
    TextureAtlas();
    ~TextureAtlas();

    // Load a sprite sheet from file
    bool load(const std::string& path, int frame_width, int frame_height,
              int frames_horizontal, int frames_vertical);

    // Get UV rectangle for a specific frame index (normalized 0-1)
    Rectangle get_frame_uv(int frame_index) const;

    // Get the underlying texture
    const Texture2D& get_texture() const { return m_texture; }

    // Get frame dimensions
    int get_frame_width() const { return m_frame_width; }
    int get_frame_height() const { return m_frame_height; }
    int get_frame_count() const { return static_cast<int>(m_frames.size()); }

    bool is_loaded() const { return m_loaded; }

private:
    Texture2D m_texture;
    std::vector<Rectangle> m_frames;  // UV rectangles for each frame (pixel coords)
    int m_frame_width;
    int m_frame_height;
    int m_frames_horizontal;
    int m_frames_vertical;
    bool m_loaded;
};

} // namespace rendering
