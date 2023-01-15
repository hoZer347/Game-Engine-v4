#pragma once

#include "Data.h"

namespace loom
{
	struct Helper final : public Object
	{
		Helper();
		void assign(Task task);
		void assign(std::vector<Task> tasks);
		static void shared_assign(std::vector<Task> tasks);

	private:
		void load() override;
		void unload() override;

		std::mutex mut;
		std::thread thread;
		std::queue<Task> in;
		std::queue<Task> out;
		std::atomic<bool> KILL = false;
	};
};
