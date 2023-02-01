#pragma once

#include <vector>
#include <thread>
#include <atomic>

namespace loom
{
	typedef void(*Task)();

	struct Helper final
	{
		Helper(Task task, const char* name = "Unnamed");

		const char* name;
		void kill();

	protected:
		friend struct Loom;

		Task task;
		std::thread thread;
		static inline std::atomic<bool> KILL = false;
		static inline std::vector<Helper*> helpers;
	};
};
