#include "Timing.h"

#include "Mem_Leak.h"

#include "Engine.h"

#include <iostream>

namespace eng
{
	namespace tsk
	{
		// WAIT
		DEFINE_TASK(Wait, bool*, )
		{
			while (data);
		};
		std::unique_ptr<Task> wait(bool* b)
		{
			return Wait::create(b);
		};
		// WAIT

		// TIMERS
		struct TimerData
		{

		};
		DEFINE_THREAD(Timer, TimerData*, )
		{

		};
		std::unique_ptr<Thread> timer(Thread* thread)
		{

		};
		// TIMER
	};
};
