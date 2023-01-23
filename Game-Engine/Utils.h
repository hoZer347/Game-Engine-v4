#pragma once

#include "Data.h"
#include "Loom.h"
#include "Helper.h"

#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
using namespace glm;

#include <chrono>
#include <memory>
#include <iostream>

namespace loom
{
	typedef std::chrono::high_resolution_clock Clock;
	typedef std::chrono::steady_clock::time_point Time;

	struct Timer : Parallel
	{
		const double GetDiff_s()   const { return diff / 1000000000; };
		const double GetDiff_mls() const { return diff / 1000000; };
		const double GetDiff_mcs() const { return diff / 1000; };
		const double GetDiff_ns()  const { return diff; };

		void sync() override
		{
			diff = (double)(Clock::now() - _clock).count();
			_clock = Time(Clock::now());
		};

	private:
		Time _clock = Clock::now();
		double diff = 0;
	};

	static inline Timer TIMER;

	struct transform final
	{
	protected:
		friend struct Loom;
		struct Transform : Manage<Transform>
		{
		private:
			virtual void exec()=0;

		public:
			virtual ~Transform() { };
			static void update() { access([](Transform* transform) { transform->exec(); }); };
		};

	public:
		template <typename T>
		[[nodiscard]] static std::shared_ptr<Transform> approach(T& _start, T& _finish, const double velocity = 1, const bool dynamic = false)
		{
			return std::shared_ptr<Transform>(new Approach(_start, _finish, velocity, dynamic));
		};
		
	private:
		static inline Helper kernel { []() { Transform::update(); } , "Transform" };

		// Approach
		// On operator(), moves the _start vector towards the _finish vector at a certain velocity
		// Syncronizes with framerate
		template <typename T>
		struct Approach final : Transform, Parallel
		{
			Approach(T& _start, T& _finish, const double velocity = 1, const bool dynamic = false)
			: dynamic(dynamic),
			  velocity(velocity),
			  _start(_start),
			  __start(_start),
			  _finish(_finish),
			  __finish(_finish),
			  _move(normalize(_finish - _start) *= velocity)
			{ };

			void exec() override
			{
				if (_start == _finish)
				{
					if (!dynamic)
						delete this;
					return;
				};

				if (double diff = TIMER.GetDiff_s())
				{
					if (dynamic)
						_move = normalize(_finish - _start) *= velocity;

					_move *= diff;

					for (int i = 0; i < sizeof(T) / sizeof(float); i++)
						if (_finish[i] - _start[i] <= _move[i])
							_start[i] = _finish[i];
						else
							_start[i] += _move[i];

					_move *= 1 / diff;
				};
			};

			void sync() override
			{
				if (dynamic)
					_finish = __finish;
				__start = _start;
			};

		private:
			const bool dynamic;
			const double velocity;

			      T   _move;
			      T   _start;
			      T& __start;
			      T   _finish;
			const T& __finish;
		};
		//
	};
};
