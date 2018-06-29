// EXTERNAL INCLUDES
// INTERNAL INCLUDES
#include "application.h"

/**
 * @brief	Entry point :)
 * 
 * @param	argc contains the number of start arguments
 * @param	argv contains the start arguments as a list of strings
 * @return	uint32 is the error code after execution (unused here)
 */
#ifdef _DEBUG
uint32 main(uint32 argc, char** argv)
#else
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	Application app;

	app.Init("[Patrick Kurras] GPU Particle Simulation", { 1366, 768 });
	app.Update();
	app.Close();

	return 0;
}
