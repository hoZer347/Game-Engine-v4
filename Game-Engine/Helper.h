#pragma once

#include "Data.h"

#include <mutex>
#include <thread>
#include <queue>
#include <atomic>
#include <functional>
#include <unordered_map>

namespace loom
{
	typedef std::function<void()> Task;

	struct Helper final
	{
		Helper();
		virtual ~Helper();
		void assign(Task task);
		void assign(std::vector<Task> tasks);
		static void shared_assign(std::vector<Task> tasks);

	private:
		std::mutex mut;
		std::thread thread;
		std::queue<Task> in;
		std::queue<Task> out;
		std::atomic<bool> KILL = false;
	};



	struct SyncHelper final
	{
		SyncHelper(Task task);
		virtual ~SyncHelper();

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
		static inline std::vector<SyncHelper*> helpers;
	};
};
