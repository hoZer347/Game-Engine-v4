#include "HelperTasks.h"

#include "Mem_Leak.h"

#include <iostream>

namespace eng
{
	namespace tsk
	{
		DEFINE_TASK(Wait, bool*, )
		{
			while (data);
		};

		std::unique_ptr<Task> wait(bool* b)
		{
			return Wait::create(b);
		};
	};
};
