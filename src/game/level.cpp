#include "game/level.h"
#include <cmath>

namespace game {

Level::Level() {
}

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

    // Create a simple rectangular room
    Sector room;
    room.floor_height = 0.0f;
    room.ceiling_height = 3.0f;
    room.floor_texture = 0;
    room.ceiling_texture = 0;
    room.light_level = 1.0f;

    // Room vertices (10x10 room)
    room.vertices.push_back(Vertex(-5.0f, -5.0f));
    room.vertices.push_back(Vertex(5.0f, -5.0f));
    room.vertices.push_back(Vertex(5.0f, 5.0f));
    room.vertices.push_back(Vertex(-5.0f, 5.0f));

    // Room walls
    for (size_t i = 0; i < 4; ++i) {
        Wall wall;
        wall.vertex_a = static_cast<uint32_t>(i);
        wall.vertex_b = static_cast<uint32_t>((i + 1) % 4);
        wall.texture_id = 0;
        wall.portal_id = -1;  // Solid wall
        room.walls.push_back(wall);
    }

    level.add_sector(room);

    // Add player spawn point in the center
    EntitySpawn player_spawn;
    player_spawn.position = {0.0f, 1.7f, 0.0f};
    player_spawn.entity_type = 0;  // Player
    player_spawn.rotation = 0.0f;
    level.add_entity_spawn(player_spawn);

    return level;
}

} // namespace game
