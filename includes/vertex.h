#pragma once

// EXTERNAL INCLUDES
// INTERNAL INCLUDES
#include "math/vec2.h"
#include "math/vec3.h"

/**
 * @brief   This structure defines a simple vertex
 *          Only positions are used for this
 */
struct Vertex
{
    Math::Vec3 position;    /**< Defines the world-space position of the vertex */
    Math::Vec3 color;       /**< Defines a per vertex color for this vertex */
    Math::Vec2 texcoord;    /**< Defines uv coordinates of textures for this vertex */
};
