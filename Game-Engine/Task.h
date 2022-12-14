#pragma once

#include <functional>

namespace loom
{
	struct Task
	{
		Task(auto task)
		: task(task)
		{ };

		inline void operator=(Task& task) { this->task = task; };
		inline void operator()() { task(); };

	private:
		std::function<void()> task;
	};
};
