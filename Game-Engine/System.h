#pragma once

#include <functional>
#include <barrier>
#include <vector>
#include <string>
#include <mutex>

#ifndef NUMBER_OF_THREADS
static inline const unsigned int NUM_THREADS = std::thread::hardware_concurrency();
#else
#define NUM_THREADS NUMBER_OF_THREADS
#endif

namespace loom
{
	template <typename T, size_t ID>
	struct System
	{
		static void allocate(size_t amount, auto... args)
		{
			mut.lock();
			_toAdd.reserve(amount);
			for (auto i = 0; i < amount; i++)
				_toAdd.emplace_back(args...);
			mut.unlock();
		};

	private:
		System() { };

		static void edit(std::function<void(T&)> f)
		{
			for (T& data : _mutable)
				f(data);
			barrier.arrive_and_wait();
		};

		static void view(std::function<void(const T&)> f)
		{
			for (const T& data : _immutable)
				f(data);
			barrier.arrive_and_wait();
		};

		static inline std::mutex mut;
		static inline std::vector<T> _toAdd;
		static inline std::vector<T> _mutable;
		static inline std::vector<T> _immutable;
		static inline bool KILL = false, PAUSE = false;
		static inline std::barrier barrier = std::barrier(NUM_THREADS, []() noexcept
		{
			System<T, ID>::_mutable.swap(System<T, ID>::_immutable);

			mut.lock();
			if (!_toAdd.empty())
			{
				_mutable.insert(_mutable.end(), _toAdd.begin(), _toAdd.end());
				_immutable.insert(_immutable.end(), _toAdd.begin(), _toAdd.end());
			};
			mut.unlock();
		});
	};
};
