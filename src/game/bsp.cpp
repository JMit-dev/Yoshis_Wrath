#include "game/bsp.h"
#include <cmath>
#include <limits>

namespace game {

BSPTree::BSPTree()
    : m_root(nullptr) {
}

void BSPTree::build_from_level(const Level& level) {
    // Collect all sector indices
    std::vector<uint32_t> all_sectors;
    const auto& sectors = level.get_sectors();
    for (size_t i = 0; i < sectors.size(); ++i) {
        all_sectors.push_back(static_cast<uint32_t>(i));
    }

    // Build tree recursively
    m_root = build_node(all_sectors, level, 0);
}

std::unique_ptr<BSPNode> BSPTree::build_node(const std::vector<uint32_t>& sector_indices,
                                              const Level& level, int depth) {
    // Create new node
    auto node = std::make_unique<BSPNode>();

    // Base case: single sector or max depth reached
    if (sector_indices.size() <= 1 || depth > 16) {
        // Leaf node - store all sectors
        node->sector_indices = sector_indices;
        return node;
    }

    // Choose a splitter line from the sectors' walls
    node->splitter = choose_splitter(sector_indices, level);

    // Partition sectors
    std::vector<uint32_t> front_sectors;
    std::vector<uint32_t> back_sectors;
    std::vector<uint32_t> spanning_sectors;

    partition_sectors(node->splitter, sector_indices, level,
                     front_sectors, back_sectors, spanning_sectors);

    // Add spanning sectors to both sides
    for (uint32_t idx : spanning_sectors) {
        front_sectors.push_back(idx);
        back_sectors.push_back(idx);
    }

    // Recursively build child nodes
    if (!front_sectors.empty()) {
        node->front = build_node(front_sectors, level, depth + 1);
    }

    if (!back_sectors.empty()) {
        node->back = build_node(back_sectors, level, depth + 1);
    }

    return node;
}

BSPSplitter BSPTree::choose_splitter(const std::vector<uint32_t>& sector_indices,
                                      const Level& level) const {
    // Simple heuristic: use the first wall of the first sector
    // More sophisticated: evaluate all walls and choose best split
    if (sector_indices.empty()) {
        return BSPSplitter();
    }

    const auto& sectors = level.get_sectors();
    const Sector& first_sector = sectors[sector_indices[0]];

    if (first_sector.walls.empty() || first_sector.vertices.size() < 2) {
        return BSPSplitter();
    }

    // Use first wall as splitter
    const Wall& wall = first_sector.walls[0];
    const Vertex& v1 = first_sector.vertices[wall.vertex_a];
    const Vertex& v2 = first_sector.vertices[wall.vertex_b];

    // Calculate direction and normalize
    float dx = v2.x - v1.x;
    float dz = v2.z - v1.z;
    float length = sqrtf(dx * dx + dz * dz);

    if (length > 0.0f) {
        dx /= length;
        dz /= length;
    }

    return BSPSplitter(v1.x, v1.z, dx, dz);
}

int BSPTree::classify_point(const BSPSplitter& splitter, float x, float z) const {
    // Calculate which side of the line the point is on
    // Using cross product: (point - line_point) x line_direction
    float dx = x - splitter.x;
    float dz = z - splitter.z;
    float cross = dx * splitter.dz - dz * splitter.dx;

    const float epsilon = 0.001f;

    if (cross > epsilon) {
        return 1;  // Front
    } else if (cross < -epsilon) {
        return -1; // Back
    }
    return 0; // On line
}

void BSPTree::partition_sectors(const BSPSplitter& splitter,
                                const std::vector<uint32_t>& sector_indices,
                                const Level& level,
                                std::vector<uint32_t>& front_sectors,
                                std::vector<uint32_t>& back_sectors,
                                std::vector<uint32_t>& spanning_sectors) const {
    const auto& sectors = level.get_sectors();

    for (uint32_t idx : sector_indices) {
        const Sector& sector = sectors[idx];

        // Classify all vertices of the sector
        int front_count = 0;
        int back_count = 0;

        for (const Vertex& v : sector.vertices) {
            int classification = classify_point(splitter, v.x, v.z);
            if (classification > 0) {
                front_count++;
            } else if (classification < 0) {
                back_count++;
            }
        }

        // Determine which side the sector belongs to
        if (back_count == 0) {
            // All vertices in front or on line
            front_sectors.push_back(idx);
        } else if (front_count == 0) {
            // All vertices in back or on line
            back_sectors.push_back(idx);
        } else {
            // Sector spans the partition line
            spanning_sectors.push_back(idx);
        }
    }
}

void BSPTree::get_visible_sectors(const Vector3& camera_pos,
                                  std::vector<uint32_t>& visible_sectors) const {
    visible_sectors.clear();

    if (m_root) {
        traverse_node(m_root.get(), camera_pos, visible_sectors);
    }
}

void BSPTree::traverse_node(const BSPNode* node, const Vector3& camera_pos,
                           std::vector<uint32_t>& visible_sectors) const {
    if (!node) {
        return;
    }

    // If leaf node, add all sectors
    if (node->is_leaf()) {
        for (uint32_t idx : node->sector_indices) {
            visible_sectors.push_back(idx);
        }
        return;
    }

    // Classify camera position relative to splitter
    int side = classify_point(node->splitter, camera_pos.x, camera_pos.z);

    // Traverse front-to-back for proper rendering order
    if (side >= 0) {
        // Camera is on front side
        traverse_node(node->front.get(), camera_pos, visible_sectors);
        traverse_node(node->back.get(), camera_pos, visible_sectors);
    } else {
        // Camera is on back side
        traverse_node(node->back.get(), camera_pos, visible_sectors);
        traverse_node(node->front.get(), camera_pos, visible_sectors);
    }
}

} // namespace game
