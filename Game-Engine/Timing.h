#pragma once

#include <memory>

namespace eng
{
	class Thread;
	class Task;

	namespace tmg
	{
		// Generates a task that waits for b to be true
		std::unique_ptr<Task> wait(bool* b);

	};
};
