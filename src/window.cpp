// EXTERNAL INCLUDES
#include <cstdio>
#include <Windows.h>
#include <Windowsx.h>
// INTERNAL INCLUDES
#include "inputevent.h"
#include "utils.h"
#include "window.h"

// Reference Callbacks here
extern MouseClickedScreenSpaceCallback OnMouseClickedScreenSpace;
extern UpArrowPressedCallback OnUpArrowPressed;
extern DownArrowPressedCallback OnDownArrowPressed;
extern RightArrowPressedCallback OnRightArrowPressed;
extern LeftArrowPressedCallback OnLeftArrowPressed;

// Screen resolution for the mouse position callback
float gScreenWidth;
float gScreenHeight;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			// Get Mouse parameters
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			
			// Calculate Imagespace to Screenspace
			Math::Vec2 screenSpacePos = {
				(((float)xPos / gScreenWidth) - 0.5f) * 2.0f,
				-(((float)yPos / gScreenHeight) - 0.5f) * 2.0f
			};

			// Left Mouse Pressed
			if (OnMouseClickedScreenSpace)
				(*OnMouseClickedScreenSpace)(screenSpacePos);

			break;
		}
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_UP:
				// Up Arrow Pressed
				if (OnUpArrowPressed)
					(*OnUpArrowPressed)();
				break;
			case VK_DOWN:
				// Down Arrow Pressed
				if (OnDownArrowPressed)
					(*OnDownArrowPressed)();
				break;
			case VK_RIGHT:
				// Right Arrow Pressed
				if (OnRightArrowPressed)
					(*OnRightArrowPressed)();
				break;
			case VK_LEFT:
				// Left Arrow Pressed
				if (OnLeftArrowPressed)
					(*OnLeftArrowPressed)();
				break;
			}
			break;
		}
		default:
			break;
	}

	// Forward everything else to DefWindowProc for processing
	return DefWindowProc(hWnd, message, wParam, lParam);
}

Window::Window(const char* title, Math::Vec2 resolution) :
	handle(0),
	isClosed(true)
{
	// Just initialize here
	Initialize(title, resolution);
}

void Window::Initialize(const char* title, Math::Vec2 res)
{
	LOG("Opening window");

	// Create a center-screen rectangle
	RECT rect = {
		static_cast<int>((GetSystemMetrics(SM_CXSCREEN) - res.x) / 2),
		static_cast<int>((GetSystemMetrics(SM_CYSCREEN) - res.y) / 2),
		rect.left + static_cast<int>(res.x),
		rect.top + static_cast<int>(res.y)
	};

	// Store the resolution in globals
	gScreenWidth = res.x;
	gScreenHeight = res.y;

	// Define the window style
	uint windowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	// Define settings for the new window class
	WNDCLASSEXA wc		= { 0 };
	wc.cbSize			= sizeof(WNDCLASSEXA);
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)COLOR_WINDOW;
	wc.hIcon			= NULL;
	wc.hInstance		= GetModuleHandleA(NULL);
	wc.lpszClassName	= title;
	wc.lpfnWndProc		= WindowProc;

	// Create the window class
	if (FAILED(RegisterClassExA(&wc)))
	{
		printf("RegisterClassExA has failed.");
		return;
	}

	// Adjust the window rectangle (depends on border style size)
	if (!(AdjustWindowRect(&rect, windowStyle, false)))
		printf("Could not adjust size of the window.");

	// Create the window now
	this->handle = reinterpret_cast<uint64>(CreateWindowExA(
		NULL,
		wc.lpszClassName,
		title,
		windowStyle,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL, NULL,
		wc.hInstance,
		NULL
	));

	// Did it work? Do we have a handle?
	if (!this->handle)
	{
		printf("CreateWindowA has failed.");
		return;
	}

	// Then show the window and update it once
	ShowWindow(reinterpret_cast<HWND>(this->handle), SW_SHOWNORMAL);
	UpdateWindow(reinterpret_cast<HWND>(this->handle));

	this->isClosed = false;
}

void Window::PumpMessages()
{
	MSG msg;
	// Process window messages
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		if (msg.message == WM_QUIT)
			this->isClosed = true;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

uint64 Window::GetHandle(void) const
{
	return this->handle;
}