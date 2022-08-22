#pragma once

#include <queue>

#include <functional>

#include <memory>
#include <thread>
#include <mutex>

#include <iostream>

namespace eng
{
	typedef std::function<bool()> Task;

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
