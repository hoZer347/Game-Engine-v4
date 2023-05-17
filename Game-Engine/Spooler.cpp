#include "Spooler.h"

#include <vector>
#include <thread>
#include <atomic>

#define MAX_TASKS 256

#ifndef NUM_THREADS
#define NUM_THREADS 4
#endif

namespace loom
{
	static inline std::atomic<bool> KILL;
	static inline std::vector<std::thread> threads;
	static inline std::vector<std::atomic<Task*>*> tasks{ 256 };

	static_assert(NUM_THREADS < MAX_TASKS);
	static_assert
	(
		NUM_THREADS == 1 ||
		NUM_THREADS == 2 ||
		NUM_THREADS == 4 ||
		NUM_THREADS == 8 ||
		NUM_THREADS == 16 ||
		NUM_THREADS == 32 ||
		NUM_THREADS == 64
	);

	void Spooler::Init()
	{
		for (auto i = 0; i < MAX_TASKS; i++)
			tasks[i] = new std::atomic<Task*>;
		for (auto i = 0; i < NUM_THREADS; i++)
			threads.emplace_back([i]()
			{
				Task* _task = nullptr;
				uint8_t _i = i;
				
				while (!KILL)
				{
					if (_task = tasks[_i]->exchange(_task))
					{
						(*_task)();
						_task = nullptr;
					};

					_i += NUM_THREADS;
				};
			});
	};
	void Spooler::Push(Task& task, int8_t&& thread_id)
	{
		uint8_t i;
		Task* _task = &task;

		if (thread_id == -1)
		{
			i = 0;

			while (_task)
			{
				_task = tasks[i]->exchange(_task);
				i++;
			};
		}
		else
		{
			i = thread_id;

			while (_task)
			{
				_task = tasks[i]->exchange(_task);
				i += NUM_THREADS;
			};
		};
	};
	void Spooler::Wait()
	{
		while (!KILL);
	};
	void Spooler::Kill()
	{
		KILL = true;
		for (auto& thread : threads)
			thread.join();
		
		KILL = false;
		threads.clear();
		tasks.clear();
	};
};
