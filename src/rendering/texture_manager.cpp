#include "rendering/texture_manager.h"
#include "platform/file_system.h"
#include <stdexcept>

namespace rendering {

TextureManager::TextureManager()
    : m_next_id(1) {  // Start at 1, reserve 0 for default
    create_default_texture();
}

TextureManager::~TextureManager() {
    unload_all();
}

void TextureManager::create_default_texture() {
    // Create a simple 2x2 white texture
    Image img = GenImageColor(2, 2, WHITE);
    m_default_texture = LoadTextureFromImage(img);
    UnloadImage(img);

    // Register as ID 0
    m_textures[0] = m_default_texture;
}

uint32_t TextureManager::load_texture(const std::string& path) {
    // Check if already loaded
    auto it = m_path_to_id.find(path);
    if (it != m_path_to_id.end()) {
        return it->second;
    }

    // Try to load the texture using platform-agnostic path
    std::string full_path = platform::FileSystem::join_path(
        platform::FileSystem::get_assets_path(), path);
    Texture2D texture = LoadTexture(full_path.c_str());

    // If loading failed, return default texture
    if (texture.id == 0) {
        return 0;
    }

    // Assign new ID and store
    uint32_t id = m_next_id++;
    m_textures[id] = texture;
    m_path_to_id[path] = id;

    return id;
}

const Texture2D& TextureManager::get_texture(uint32_t id) const {
    auto it = m_textures.find(id);
    if (it != m_textures.end()) {
        return it->second;
    }
    return m_default_texture;
}

bool TextureManager::has_texture(uint32_t id) const {
    return m_textures.find(id) != m_textures.end();
}

void TextureManager::unload_all() {
    for (auto& pair : m_textures) {
        if (pair.first != 0) {  // Don't unload default texture yet
            UnloadTexture(pair.second);
        }
    }
    m_textures.clear();
    m_path_to_id.clear();

    // Unload default texture last
    UnloadTexture(m_default_texture);
}

} // namespace rendering
