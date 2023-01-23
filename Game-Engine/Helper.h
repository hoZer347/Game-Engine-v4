#pragma once

#include "Data.h"

#include <mutex>
#include <queue>
#include <thread>
#include <atomic>
#include <functional>

namespace loom
{
	typedef std::function<void()> Task;

	struct Helper final : public Manage<Helper>
	{
		Helper(Task task);
		virtual ~Helper();

		static inline void kill() { KILL = true; };

	protected:
		friend struct Loom;
		union _input
		{
			uint16_t data[4] = { 0, 0, 0, 0 };
			uint64_t input;
		};

		Task task;
		std::thread thread;
		static inline std::atomic<bool> KILL = false;
	};
};
