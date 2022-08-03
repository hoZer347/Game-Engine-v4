#pragma once

#include <queue>

#include <functional>

#include <memory>
#include <thread>
#include <mutex>

namespace eng
{
	typedef std::function<bool()> Task;

	struct Thread
	{
		virtual ~Thread();

		void push(Task task);
		void push(Task* task);
		virtual void on_kill() { };

		static Thread* create();

		std::thread thread;

	protected:
		Thread();

		bool KILL = false;

		std::queue<Task*> to_add;
		std::queue<Task*> tasks;
	};
};
