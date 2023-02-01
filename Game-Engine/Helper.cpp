#include "Helper.h"

#include <iostream>

namespace loom
{
	Helper::Helper(Task task, const char* name)
	: task(task), name(name)
	{
		helpers.push_back(this);
	};
	void Helper::kill()
	{
		KILL = true;
		if (thread.joinable())
			thread.join();
	};
};
