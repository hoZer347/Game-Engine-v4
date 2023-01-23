#pragma once

#include "Data.h"

#include <thread>
#include <atomic>
#include <functional>

namespace loom
{
	typedef std::function<void()> Task;

	struct Helper final : public Manage<Helper>
	{
		Helper(Task task, std::string name = "Unnamed");
		
		const std::string name;

		void kill();

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
