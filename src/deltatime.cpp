// EXTERNAL INCLUDES
#include <algorithm>
#include <chrono>
// INTERNAL INCLUDES
#include "deltatime.h"

void Time::GetDetlaTime(Time& time)
{
	// calculate delta time in milliseconds
	std::chrono::time_point<std::chrono::high_resolution_clock> now =
		std::chrono::high_resolution_clock::now();
	auto duration = now.time_since_epoch();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

	time.newTime = milliseconds;
	time.deltaTime = static_cast<float>(time.newTime - time.oldTime);
	time.deltaTime = std::min(time.deltaTime, maxTimeStep);

	// very inaccurate but it keeps the
	// clock from hitting zero
	// (zero delta not allowed!)
	if (time.deltaTime <= 0.00001f)
		time.deltaTime = 0.00001f;

	time.oldTime = time.newTime;
}