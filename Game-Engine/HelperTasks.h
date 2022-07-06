#pragma once

#include "Engine.h"

#include <memory>

namespace eng
{
	namespace tsk
	{
		// Generates a task that waits for b to be true
		std::unique_ptr<Task> wait(bool* b);
	};
};
