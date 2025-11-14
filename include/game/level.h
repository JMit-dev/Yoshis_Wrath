#pragma once

#include "raylib.h"
#include <vector>
#include <cstdint>

namespace game {

// Forward declarations
struct Sector;
struct Wall;
struct Portal;

// A 2D vertex in the level
struct Vertex {
    float x;
    float z;

    Vertex() : x(0.0f), z(0.0f) {}
    Vertex(float x_val, float z_val) : x(x_val), z(z_val) {}
};

// A wall connecting two vertices
struct Wall {
    uint32_t vertex_a;      // Index into sector's vertex list
    uint32_t vertex_b;      // Index into sector's vertex list
    uint32_t texture_id;    // Texture to use for this wall
    int32_t portal_id;      // -1 if solid wall, otherwise index to portal

    Wall()
        : vertex_a(0)
        , vertex_b(0)
        , texture_id(0)
        , portal_id(-1) {}
};

// A portal connecting two sectors
struct Portal {
    uint32_t target_sector; // Index of the sector this portal leads to
    float floor_height;     // Floor height of portal opening
    float ceiling_height;   // Ceiling height of portal opening

    Portal()
        : target_sector(0)
        , floor_height(0.0f)
        , ceiling_height(0.0f) {}
};

// A convex sector (room/area) in the level
struct Sector {
    std::vector<Vertex> vertices;
    std::vector<Wall> walls;

    float floor_height;
    float ceiling_height;
    uint32_t floor_texture;
    uint32_t ceiling_texture;

    // Lighting (0.0 = dark, 1.0 = full brightness)
    float light_level;

    Sector()
        : floor_height(0.0f)
        , ceiling_height(3.0f)
        , floor_texture(0)
        , ceiling_texture(0)
        , light_level(1.0f) {}
};

// Entity spawn point
struct EntitySpawn {
    Vector3 position;
    uint32_t entity_type;
    float rotation;

    EntitySpawn()
        : position{0.0f, 0.0f, 0.0f}
        , entity_type(0)
        , rotation(0.0f) {}
};

// Complete level data
class Level {
public:
    Level();
    ~Level() = default;

    // Level building (for now, manual - editor will come later)
    uint32_t add_sector(const Sector& sector);
    uint32_t add_portal(const Portal& portal);
    void add_entity_spawn(const EntitySpawn& spawn);

    // Getters
    const std::vector<Sector>& get_sectors() const { return m_sectors; }
    const std::vector<Portal>& get_portals() const { return m_portals; }
    const std::vector<EntitySpawn>& get_spawns() const { return m_entity_spawns; }
    const Sector& get_sector(uint32_t index) const { return m_sectors[index]; }

    // Find which sector contains a point
    int32_t find_sector_at_point(float x, float z) const;

    // Create a simple test level
    static Level create_test_level();

private:
    std::vector<Sector> m_sectors;
    std::vector<Portal> m_portals;
    std::vector<EntitySpawn> m_entity_spawns;

    bool point_in_sector(const Sector& sector, float x, float z) const;
};

} // namespace game
