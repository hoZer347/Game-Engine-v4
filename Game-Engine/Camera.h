#pragma once

#include "Task.h"

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	struct Camera
	{
		Task _update();

		mat4	trns = mat4(1),
				yaww = mat4(1),
				ptch = mat4(1),
				roll = mat4(1);

		vec3	eye = vec3(0, 0, 1),
				ctr = vec3(0, 0, 0),
				up  = vec3(0, 1, 0);

		float	fovy			= 90.f,
				near			= .1f,
				far				= 1000.f;

	private:
		int		w=1, h=1;
		
		mat4	mvp  = mat4(1),
				mode = mat4(1),
				view = mat4(1),
				proj = mat4(1),
				rotn = mat4(1);
	};
};
