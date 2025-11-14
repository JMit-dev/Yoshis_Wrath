#pragma once

#include "game/level.h"
#include <vector>
#include <memory>
#include <cstdint>

namespace game {

// BSP splitter plane (2D since we're doing Doom-style BSP)
struct BSPSplitter {
    float x;        // Point on the line
    float z;
    float dx;       // Direction vector (normalized)
    float dz;

    BSPSplitter() : x(0.0f), z(0.0f), dx(1.0f), dz(0.0f) {}
    BSPSplitter(float x_val, float z_val, float dx_val, float dz_val)
        : x(x_val), z(z_val), dx(dx_val), dz(dz_val) {}
};

// BSP node in the tree
struct BSPNode {
    BSPSplitter splitter;

    // Indices of sectors visible from this node
    std::vector<uint32_t> sector_indices;

    // Child nodes (nullptr if leaf)
    std::unique_ptr<BSPNode> front;
    std::unique_ptr<BSPNode> back;

    bool is_leaf() const {
        return front == nullptr && back == nullptr;
    }

    BSPNode() : splitter(), sector_indices(), front(nullptr), back(nullptr) {}
};

// BSP tree for level spatial partitioning
class BSPTree {
public:
    BSPTree();
    ~BSPTree() = default;

    // Build BSP tree from level sectors
    void build_from_level(const Level& level);

    // Get visible sectors from a point (for rendering)
    void get_visible_sectors(const Vector3& camera_pos,
                            std::vector<uint32_t>& visible_sectors) const;

    // Get the root node
    const BSPNode* get_root() const { return m_root.get(); }

    // Check if tree is built
    bool is_built() const { return m_root != nullptr; }

private:
    std::unique_ptr<BSPNode> m_root;

    // Recursively build BSP tree
    std::unique_ptr<BSPNode> build_node(const std::vector<uint32_t>& sector_indices,
                                        const Level& level, int depth);

    // Choose best splitter from available walls
    BSPSplitter choose_splitter(const std::vector<uint32_t>& sector_indices,
                               const Level& level) const;

    // Classify a point relative to a splitter
    // Returns: -1 = back, 0 = on line, 1 = front
    int classify_point(const BSPSplitter& splitter, float x, float z) const;

    // Partition sectors based on splitter
    void partition_sectors(const BSPSplitter& splitter,
                          const std::vector<uint32_t>& sector_indices,
                          const Level& level,
                          std::vector<uint32_t>& front_sectors,
                          std::vector<uint32_t>& back_sectors,
                          std::vector<uint32_t>& spanning_sectors) const;

    // Recursively traverse tree to find visible sectors
    void traverse_node(const BSPNode* node, const Vector3& camera_pos,
                      std::vector<uint32_t>& visible_sectors) const;
};

} // namespace game
