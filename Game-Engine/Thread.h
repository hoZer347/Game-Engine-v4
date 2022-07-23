#pragma once

#include <functional>
#include <mutex>
#include <thread>
#include <unordered_set>
#include <vector>
#include <queue>
#include <stdarg.h>

#include <iostream>

namespace eng
{
	typedef const unsigned int ThreadID;

	static inline std::mutex thread_create_mutex;
	static inline std::unordered_set<unsigned int> IDs;

	template <ThreadID ID>
	struct Thread
	{
		static void init()
		{
			thread_create_mutex.lock();
			if (IDs.count(ID))
			{
				thread_create_mutex.unlock();
				return;
			};
			IDs.insert(ID);
			thread_create_mutex.unlock();

			thread = std::thread([]()
				{
					while (!KILL)
					{
						while (!tasks.empty())
						{
							tasks.front()();
							tasks.pop();
						};

						for (auto& task : kernel)
							task();
					};
				});
		};

		static void push(std::function<void()> f)
		{
			tasks_access.lock();
			tasks.emplace(f);
			tasks_access.unlock();
		};

		static void assign(std::function<void()> f)
		{
			tasks_access.lock();
			tasks.emplace([f]() { kernel.emplace_back(f); });
			tasks_access.unlock();
		};

		static void join()
		{
			thread.join();
		};

	protected:
		friend class Engine;
		static inline bool KILL = false;

	private:
		static inline std::thread							thread;
		static inline std::queue  <std::function<void()>>	tasks;
		static inline std::vector <std::function<void()>>	kernel;
		static inline std::mutex							tasks_access;
	};
};
