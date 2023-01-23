#include "Helper.h"

#include <iostream>

namespace loom
{
	Helper::Helper(Task task)
	: task(task)
	{ };
	Helper::~Helper()
	{
		KILL = true;
		if (thread.joinable())
			thread.join();
	};
};
