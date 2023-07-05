#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
using namespace glm;

namespace loom
{
	template <auto frequency = 0>
	void Animate_MoveTowards(vec4& pos, const vec4 goal, float speed)
	{
		if constexpr (frequency)
		{
			static Timer timer;

			if (timer.GetDiff_mls() < 1000.f / frequency)
				return;
			timer.push(std::chrono::milliseconds((long long)(1000.f / frequency)));
		};

		vec4 _pos = pos;

		if (_pos != goal) _pos += normalize(goal - _pos) * speed;
		if (std::abs(_pos.x - goal.x) < speed) _pos.x = goal.x;
		if (std::abs(_pos.y - goal.y) < speed) _pos.y = goal.y;
		if (std::abs(_pos.z - goal.z) < speed) _pos.z = goal.z;

		pos = _pos;
	};
};
