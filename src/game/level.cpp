#include "game/level.h"
#include "game/bsp.h"
#include <cmath>

namespace game {

Level::Level()
    : m_bsp_tree(nullptr) {
}

Level::~Level() = default;

uint32_t Level::add_sector(const Sector& sector) {
    m_sectors.push_back(sector);
    return static_cast<uint32_t>(m_sectors.size() - 1);
}

uint32_t Level::add_portal(const Portal& portal) {
    m_portals.push_back(portal);
    return static_cast<uint32_t>(m_portals.size() - 1);
}

void Level::add_entity_spawn(const EntitySpawn& spawn) {
    m_entity_spawns.push_back(spawn);
}

void Level::build_bsp() {
    m_bsp_tree = std::make_unique<BSPTree>();
    m_bsp_tree->build_from_level(*this);
}

int32_t Level::find_sector_at_point(float x, float z) const {
    for (size_t i = 0; i < m_sectors.size(); ++i) {
        if (point_in_sector(m_sectors[i], x, z)) {
            return static_cast<int32_t>(i);
        }
    }
    return -1;
}

bool Level::point_in_sector(const Sector& sector, float x, float z) const {
    // Simple ray casting algorithm to check if point is inside polygon
    // Cast a ray from the point to the right and count intersections
    int intersections = 0;
    size_t vertex_count = sector.vertices.size();

    for (size_t i = 0; i < vertex_count; ++i) {
        size_t next = (i + 1) % vertex_count;
        const Vertex& v1 = sector.vertices[i];
        const Vertex& v2 = sector.vertices[next];

        // Check if ray intersects this edge
        if ((v1.z > z) != (v2.z > z)) {
            float x_intersect = v1.x + (z - v1.z) * (v2.x - v1.x) / (v2.z - v1.z);
            if (x < x_intersect) {
                intersections++;
            }
        }
    }

    return (intersections % 2) == 1;
}

Level Level::create_test_level() {
    Level level;

    // Create first room (starting room)
    Sector room1;
    room1.floor_height = 0.0f;
    room1.ceiling_height = 3.0f;
    room1.floor_texture = 0;
    room1.ceiling_texture = 0;
    room1.light_level = 1.0f;

    // Room 1 vertices (10x10 room)
    room1.vertices.push_back(Vertex(-5.0f, -5.0f));  // 0
    room1.vertices.push_back(Vertex(5.0f, -5.0f));   // 1
    room1.vertices.push_back(Vertex(5.0f, 5.0f));    // 2
    room1.vertices.push_back(Vertex(-5.0f, 5.0f));   // 3

    // Room 1 walls - wall 1 (right side) will be a portal
    for (size_t i = 0; i < 4; ++i) {
        Wall wall;
        wall.vertex_a = static_cast<uint32_t>(i);
        wall.vertex_b = static_cast<uint32_t>((i + 1) % 4);
        wall.texture_id = 0;
        // Wall 1 (right side) is a portal to room 2
        wall.portal_id = (i == 1) ? 0 : -1;
        room1.walls.push_back(wall);
    }

    level.add_sector(room1);

    // Create second room (connected via portal)
    Sector room2;
    room2.floor_height = 0.0f;
    room2.ceiling_height = 3.0f;
    room2.floor_texture = 0;
    room2.ceiling_texture = 0;
    room2.light_level = 1.0f;

    // Room 2 vertices (8x10 room, connected to right side of room1)
    room2.vertices.push_back(Vertex(5.0f, -5.0f));   // 0 (shared with room1)
    room2.vertices.push_back(Vertex(13.0f, -5.0f));  // 1
    room2.vertices.push_back(Vertex(13.0f, 5.0f));   // 2
    room2.vertices.push_back(Vertex(5.0f, 5.0f));    // 3 (shared with room1)

    // Room 2 walls - wall 3 (left side) will be portal back to room 1
    for (size_t i = 0; i < 4; ++i) {
        Wall wall;
        wall.vertex_a = static_cast<uint32_t>(i);
        wall.vertex_b = static_cast<uint32_t>((i + 1) % 4);
        wall.texture_id = 0;
        // Wall 3 (left side) is a portal back to room 1
        wall.portal_id = (i == 3) ? 1 : -1;
        room2.walls.push_back(wall);
    }

    level.add_sector(room2);

    // Create portal connections
    Portal portal1;
    portal1.target_sector = 1;
    portal1.floor_height = 0.0f;
    portal1.ceiling_height = 3.0f;
    level.add_portal(portal1);

    Portal portal2;
    portal2.target_sector = 0;
    portal2.floor_height = 0.0f;
    portal2.ceiling_height = 3.0f;
    level.add_portal(portal2);

    // Add player spawn point in center of first room
    EntitySpawn player_spawn;
    player_spawn.position = {0.0f, 1.7f, 0.0f};
    player_spawn.entity_type = 0;  // Player
    player_spawn.rotation = 0.0f;
    level.add_entity_spawn(player_spawn);

    // Build BSP tree
    level.build_bsp();

    return level;
}

} // namespace game
