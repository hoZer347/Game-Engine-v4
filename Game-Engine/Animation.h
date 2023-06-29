#pragma once

#include "Data.h"
#include "Timing.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
using namespace glm;

namespace loom
{
	void Animate_MoveTowards(vec4& pos, const vec4 goal, float speed, float frequency = 60)
	{
		if (pos != goal) pos += normalize(goal - pos) * speed;
		if (std::abs(pos.x - goal.x) < speed) pos.x = goal.x;
		if (std::abs(pos.y - goal.y) < speed) pos.y = goal.y;
		if (std::abs(pos.z - goal.z) < speed) pos.z = goal.z;
	};
};
