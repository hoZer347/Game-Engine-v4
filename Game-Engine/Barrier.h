#pragma once

#include "Engine.h"
#include "Task.h"

#include <iostream>

#include <mutex>
#include <memory>
#include <vector>
#include <barrier>
#include <unordered_set>

namespace loom
{
	template <size_t SIZE, size_t ID> struct _Barrier;
	struct Sync;

	struct Barrier final
	{
		template <size_t SIZE, size_t ID>
		static inline std::shared_ptr<Barrier> create()
		{
			return _Barrier<SIZE, ID>::create();
		};

		Barrier(Task& t0, Task& t1, Task& t2)
		: on_cycle(t0),
		  arrive_and_wait(t1),
		  arrive_and_drop(t2)
		{ };

	protected:
		friend struct Sync;
		Task& on_cycle;
		Task& arrive_and_wait;
		Task& arrive_and_drop;
	};

	template <size_t SIZE, size_t ID=0>
	struct _Barrier final
	{
#define _b _Barrier<SIZE, ID>::

	protected:
		static inline std::shared_ptr<Barrier> create()
		{
			mut.lock();
			if (accessed)
			{
				std::cerr << "Barrier Has Already Been Accessed" << std::endl;
				return nullptr;
			};

			accessed = true;
			mut.unlock();

			return std::make_shared<Barrier>(
				on_cycle,
				arrive_and_wait,
				arrive_and_drop);
		};

		friend struct Barrier;

		static inline std::mutex			mut;
		static inline bool accessed			= false;
		static inline Task on_cycle			= []() noexcept { };
		static inline std::barrier barrier	= std::barrier(SIZE, []() noexcept { _b on_cycle(); });
		static inline Task arrive_and_wait	= []() noexcept { _b barrier.arrive_and_wait(); };
		static inline Task arrive_and_drop	= []() noexcept { _b barrier.arrive_and_drop(); };
	};
};
