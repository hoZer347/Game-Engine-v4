#pragma once

#include <chrono>
#include <atomic>
#include <functional>
#include <type_traits>

#define TESTFOR(NAME)\
	template<typename T, typename = void>\
	struct has##NAME : std::false_type\
	{};\
	template<typename T>\
	struct has##NAME<T, decltype(std::declval<T>().##NAME(), void())> : std::true_type\
	{};\
	static inline const bool Has##NAME = has##NAME<T>::value;\



namespace loom
{
	typedef std::function<void()> Task;

	struct Utils
	{
	private:
		typedef std::chrono::high_resolution_clock Clock;
		typedef std::chrono::steady_clock::time_point Time;

	public:
		// High Resolution Timer so you don't have to interact with std::chrono lmao
		struct Timer
		{
			// Gets the difference between the last pinged time and the current one in seconds
			_NODISCARD const double GetDiff_s()   const { return (double)(Clock::now() - _clock).count() / 1000000000; };
			
			// Gets the difference between the last pinged time and the current one in milliseconds
			_NODISCARD const double GetDiff_mls() const { return (double)(Clock::now() - _clock).count() / 1000; };

			// Gets the difference between the last pinged time and the current one in microseconds
			_NODISCARD const double GetDiff_mcs() const { return (double)(Clock::now() - _clock).count() / 1000000; };
			
			// Gets the difference between the last pinged time and the current one in nanoseconds
			_NODISCARD const double GetDiff_ns()  const { return (double)(Clock::now() - _clock).count(); };

			// Adds time to the clock's last pinged time
			template <typename T>
			void push(T t) { _clock += t; }

			// Resets the last pinged time to the current time
			void restart() { _clock = Time(Clock::now()); };

		private:
			Time _clock = Clock::now();
			double diff = 0;
		};
		//
	};
};
