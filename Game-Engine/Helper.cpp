#include "Helper.h"

#include <iostream>

namespace loom
{
	Helper::Helper(Task task, std::string name)
	: task(task), name(name)
	{ };
	void Helper::kill()
	{
		KILL = true;
		if (thread.joinable())
			thread.join();
	};
};
