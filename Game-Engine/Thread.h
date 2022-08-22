#pragma once

#include <queue>

#include <functional>

#include <memory>
#include <thread>
#include <mutex>
#include <atomic>

#include <iostream>

#include <iostream>

namespace eng
{
	typedef std::function<bool()> Task;

	inline std::atomic<unsigned int> NUM_THREADS;

	struct Thread
	{
		~Thread();

		void push(Task task);
		void push(Task* task);

		static Thread* create();

		std::thread thread;

	protected:
		Thread();

		bool KILL = false;

		std::queue<Task*> to_add;
		std::queue<Task*> tasks;
	};
};
