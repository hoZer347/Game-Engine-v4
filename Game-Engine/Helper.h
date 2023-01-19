#pragma once

#include <mutex>
#include <thread>
#include <queue>
#include <atomic>
#include <functional>

namespace loom
{
	typedef std::function<void()> Task;

	struct Helper final
	{
		Helper();
		void assign(Task task);
		void assign(std::vector<Task> tasks);
		static void shared_assign(std::vector<Task> tasks);

	private:
		void load();
		void unload();

		std::mutex mut;
		std::thread thread;
		std::queue<Task> in;
		std::queue<Task> out;
		std::atomic<bool> KILL = false;
	};
};
