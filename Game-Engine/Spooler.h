#pragma once

#include <functional>

namespace loom
{
	typedef std::function<void()> Task;

	struct Spooler final
	{
		static void Init();
		static void Push(Task& task, int8_t&& thread_id = -1);
		static void Wait();
		static void Kill();
	};
};
