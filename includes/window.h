#pragma once

// EXTERNAL INCLUDES
// INTERNAL INCLUDES
#include "math/vec2.h"
#include "types.h"
#include "window.h"

class Window
{
public:

	/**
	 * @brief	Construct a new Window object
	 * 
	 * @param	title is the title bar text
	 * @param	resolution is the desired resolution of the window
	 */
	Window(const char* title, Math::Vec2 resolution);

	/**
	 * @brief 	This method initializes the window
	 * 
	 * @param	title is the title bar text
	 * @param	res is the desired resolution of the window
	 */
	void Initialize(const char* title, Math::Vec2 res);
	/**
	 * @brief	This method dispatches window messages to
	 * 			the window event handlers
	 * 			This should be called inside the main loop
	 */
	void PumpMessages(void);

	/**
	 * @brief	Retrieves the current window handle
	 * @return	uint64 is the current window handle 
	 */
	uint64 GetHandle(void) const;

	bool isClosed; /**< is the current desired state of the window */

private:

	uint64 handle;

};