#pragma once

// EXTERNAL INCLUDES
#include <stdint.h>
// INTERNAL INCLUDES
#include "math/vec2.h"

typedef void(*MouseClickedScreenSpaceCallback)(Math::Vec2 position); /**< callback for the left mouse button */
typedef void(*LeftArrowPressedCallback)(void);  /**< callback for the left arrow */
typedef void(*RightArrowPressedCallback)(void);  /**< callback for the right arrow */
typedef void(*UpArrowPressedCallback)(void);  /**< callback for the up arrow */
typedef void(*DownArrowPressedCallback)(void);  /**< callback for the down arrow */
