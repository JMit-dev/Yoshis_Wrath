#pragma once

#include "raylib.h"
#include <cstdint>

namespace rendering {

// A billboard sprite that always faces the camera
struct Sprite {
    Vector3 position;     // World position
    uint32_t texture_id;  // Texture to display
    float width;          // Width in world units
    float height;         // Height in world units
    float anchor_y;       // Vertical anchor (0.0 = bottom, 0.5 = center, 1.0 = top)

    Sprite()
        : position{0.0f, 0.0f, 0.0f}
        , texture_id(0)
        , width(1.0f)
        , height(1.0f)
        , anchor_y(0.0f) {}
};

} // namespace rendering
