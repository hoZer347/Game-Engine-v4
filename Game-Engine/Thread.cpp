#include "Thread.h"

#include "Mem_Leak.h"

static inline std::mutex mut;

namespace eng
{
	Thread::Thread()
	{
		thread = std::thread([this]()
			{
				while (!KILL)
				{
					while (tasks.size())
					{
						if ((*tasks.front())())
							tasks.push(tasks.front());

						tasks.pop();
					};

					mut.lock();
					while (to_add.size())
					{
						tasks.push(to_add.front());
						to_add.pop();
					};
					mut.unlock();
				};
				on_kill();
			});
	};

	Thread::~Thread()
	{
		KILL = true;
		thread.join();
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
