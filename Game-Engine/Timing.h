#pragma once

#include "Data.h"

#include <chrono>

namespace loom
{
	// Way of tracking time within the engine
	struct Timer final
	{
	private:
		typedef std::chrono::high_resolution_clock Clock;
		typedef std::chrono::steady_clock::time_point Time;

	public:
		// Gets the difference between the last pinged time and the current one in seconds
		_NODISCARD const double GetDiff_s()   const { return (double)(Clock::now() - _clock).count() / 1000000000.0; };
			
		// Gets the difference between the last pinged time and the current one in milliseconds
		_NODISCARD const double GetDiff_mls() const { return (double)(Clock::now() - _clock).count() / 1000.0; };

		// Gets the difference between the last pinged time and the current one in microseconds
		_NODISCARD const double GetDiff_mcs() const { return (double)(Clock::now() - _clock).count() / 1000000.0; };
			
		// Gets the difference between the last pinged time and the current one in nanoseconds
		_NODISCARD const double GetDiff_ns()  const { return (double)(Clock::now() - _clock).count(); };

		// Adds time to the clock's last pinged time
		template <typename T>
		void push(T t) { _clock += t; }

		// Resets the last pinged time to the current time
		void restart() { _clock = Time(Clock::now()); };

	private:
		Time _clock = Clock::now();
	};

	_NODISCARD inline double TimedFunc(void(*f)())
	{
		Timer TIMER;
		TIMER.restart();
		f();
		return TIMER.GetDiff_mls();
	};
};
