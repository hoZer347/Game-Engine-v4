#include "Thread.h"

#include "Mem_Leak.h"

static inline std::mutex mut;

namespace eng
{
	Thread::Thread()
	{
		thread = std::thread([this]()
			{
				tasks.push(new Task([this]()
					{
						mut.lock();
						while (to_add.size())
						{
							tasks.push(to_add.front());
							to_add.pop();
						};
						mut.unlock();

						return true;
					}));

				while (!KILL)
				{
					while (tasks.size() && !KILL)
					{
						if ((*tasks.front())())
							tasks.push(tasks.front());
						else
							delete tasks.front();

						tasks.pop();
					};
				};
			});
	};

	Thread::~Thread()
	{
		KILL = true;
		thread.join();

		mut.lock();
		while (!tasks.empty())
		{
			delete tasks.front();
			tasks.front() = nullptr;
			tasks.pop();
		};

		while (!to_add.empty())
		{
			delete to_add.front();
			to_add.front() = nullptr;
			to_add.pop();
		};
		mut.unlock();
	};

	void Thread::push(Task task)
	{
		mut.lock();
		to_add.push(new Task(task));
		mut.unlock();
	};

	void Thread::push(Task* task)
	{
		mut.lock();
		to_add.push(task);
		mut.unlock();
	};

	Thread* Thread::create()
	{
		return new Thread();
	};
};
