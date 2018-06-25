#pragma once

// EXTERNAL INCLUDES
// INTERNAL INCLUDES
#include "math/vec2.h"
#include "particlerenderer.h"
#include "types.h"

class Window;

/**
 * @brief	This is the main application class
 * 			It's purpose is the creation of the window and renderer
 */
class Application
{
public:

	/**
	 * @brief	State defines the states of the application
	 */
	enum State
	{
		Running,	/**< defines the applications running state */
		Stopped		/**< defines the applications stopped state */
	};

	~Application();

	/**
	 * @brief 	This method starts and initializes the application
	 * 			It creates a window and a D3D11 renderer.
	 * @param	title is the title of the window
	 * @param	resolution is the resolution of the window
	 */
	void Init(const char* title, Math::Vec2 resolution = { 800, 600 });
	/**
	 * @brief	This method contains the main update loop of the application.
	 * 			Call this after you called the Init function.
	 */
    void Update(void);
	/**
	 * @brief	This method closes the application after the Update
	 * 			method stops blocking.
	 */
    void Close(void);

private:

	const char* title;
	uint64 processID;
	Window* window;
	State appstate;
	ParticleRenderer* renderer;

};
