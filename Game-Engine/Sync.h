#pragma once

#include "Engine.h"
#include "Task.h"

#include <iostream>

#include <vector>
#include <barrier>

namespace loom
{
	template <size_t ID> struct Sync;

	template <size_t SIZE, size_t ID=0>
	struct Barrier final
	{
#define _b Barrier<SIZE, ID>::

	protected:
		static inline Task on_cycle = []() noexcept { };

	private:
		Barrier() { };
		static inline std::barrier barrier = std::barrier(SIZE, []() noexcept { _b on_cycle(); });

	protected:
		friend struct Sync<ID>;
		static inline Task arrive_and_wait	= []() noexcept { _b barrier.arrive_and_wait(); };
		static inline Task arrive_and_drop	= []() noexcept { _b barrier.arrive_and_drop(); };
		static inline auto arrive			= []() noexcept { return _b barrier.arrive(); };
		static inline auto wait				= [](auto t) noexcept { _b barrier.wait(t); };
	};

	template <size_t ID=0>
	struct Sync final
	{
		template <size_t NEW_SIZE>
		static inline void align(std::vector<Thread*> threads, Task on_cycle)
		{
			if (Sync<ID>::threads.size() + threads.size() != NEW_SIZE || has_been_set)
			{
				std::cerr << "Invalid NEW_SIZE" << std::endl;
				return;
			};

			Sync<ID>::resize<NEW_SIZE>();

			Sync<ID>::threads.insert(Sync<ID>::threads.end(), threads.begin(), threads.end());

			for (auto& thread : threads)
			{
				assign(thread,			[]() noexcept { arrive_and_wait(); });
				assign_on_kill(thread,	[]() noexcept { arrive_and_drop(); });
			};

			Sync<ID>::on_cycle = on_cycle;

			has_been_set = true;
		};

	private:
		Sync() { };
		template <size_t SIZE>
		static void resize()
		{
			arrive_and_wait = _b arrive_and_wait;
			arrive_and_drop = _b arrive_and_drop;
			_b on_cycle = []() noexcept { on_cycle(); };
		};
		
		static inline bool					has_been_set = false;
		static inline Task on_cycle			= []() noexcept { };
		static inline Task arrive_and_wait	= []() noexcept { };
		static inline Task arrive_and_drop  = []() noexcept { };
		static inline std::vector<Thread*>	threads;
	};
};
