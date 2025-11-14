#pragma once

#include "raylib.h"
#include <string>
#include <unordered_map>
#include <cstdint>

namespace rendering {

// Manages texture loading and caching
class TextureManager {
public:
    TextureManager();
    ~TextureManager();

    // Disable copy, allow move
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    TextureManager(TextureManager&&) = default;
    TextureManager& operator=(TextureManager&&) = default;

    // Load a texture from file and return its ID
    uint32_t load_texture(const std::string& path);

    // Get a texture by ID
    const Texture2D& get_texture(uint32_t id) const;

    // Check if texture exists
    bool has_texture(uint32_t id) const;

    // Get default white texture for untextured surfaces
    const Texture2D& get_default_texture() const { return m_default_texture; }

    // Unload all textures
    void unload_all();

private:
    std::unordered_map<uint32_t, Texture2D> m_textures;
    std::unordered_map<std::string, uint32_t> m_path_to_id;
    Texture2D m_default_texture;
    uint32_t m_next_id;

    void create_default_texture();
};

} // namespace rendering
