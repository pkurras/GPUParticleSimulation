#pragma once

// EXTERNAL INCLUDES
#include <ctime>
// INTERNAL INCLUDES
#include "types.h"

namespace Time
{
	constexpr float maxTimeStep = (1.f / 60.f); /**< this is the maximum time step that is allowed in the deltaTime */

	/**
	 * @brief	This struct defines a time counter class
	 */
	struct Time
	{
		uint64 oldTime;
		uint64 newTime;

		float deltaTime;
	};

	/**
	 * @brief	This method returns the delta time since
	 * 			the last call of this method (time since epoch is used)
	 * @param	is the time struct containing the new delta time 
	 */
	void GetDetlaTime(Time &time);
}