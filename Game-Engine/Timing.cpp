#include "Timing.h"

#include "Mem_Leak.h"

#include "Engine.h"

#include <iostream>

namespace eng
{
	namespace tmg
	{
		// WAIT
		DEFINE_TASK(Wait, bool, )
		{
			while (data);
		};
		std::unique_ptr<Task> wait(bool* b)
		{
			return Wait::create(b);
		};
		// WAIT

		// TIMERS
		// TIMER
	};
};
