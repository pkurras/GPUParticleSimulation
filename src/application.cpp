// EXTERNAL INCLUDES
#include <windows.h>
// INTERNAL INCLUDES
#include "application.h"
#include "utils.h"
#include "window.h"
#include "deltaTime.h"

Application::~Application()
{
	SAFE_DELETE(this->renderer);
	SAFE_DELETE(this->window);
}

void Application::Init(const char* title, Math::Vec2 resolution)
{
	LOG("Starting application");

	this->title = title;

	// set up the window and the renderer
	this->window = new Window(title, resolution);
	this->renderer = new ParticleRenderer();
	this->renderer->Initialize(this->window->GetHandle(), resolution);
}
void Application::Update(void)
{
	LOG("Gameloop starting");

	// create the buffers and fill the particle data in
	this->renderer->SetupParticles();

	// create a time object for the delta time
	Time::Time time = { 0 };

	do
	{
		// window message loop
		this->window->PumpMessages();

		// update of the current delta time
		Time::GetDetlaTime(time);

		// clear the frame
		this->renderer->ClearFrame();
		// update the particles
		this->renderer->UpdateParticles(time.deltaTime);
		// render the particles
		this->renderer->RenderParticles(time.deltaTime);
		// show them on screen
		this->renderer->PresentFrame();

		// in case the desired window state
		// is "closed" we stop the game loop
		if (this->window->isClosed)
		{
			LOG("Gameloop stopped");
			break;
		}

	} while (true);
}
void Application::Close(void)
{
	LOG("Application closing");
}
