#pragma once

#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
using namespace glm;

namespace eng
{
	struct Window;

	struct Camera
	{
	protected:
		friend struct Window;

		void update();

		mat4	mvp		= mat4(1),
				mode	= mat4(1),
				view	= mat4(1),
				proj	= mat4(1),
				rotn	= mat4(1),
				trns	= mat4(1),
				roll	= mat4(1),
				ptch	= mat4(1),
				yaww	= mat4(1);

		vec3	eye		= vec3(0, 0, 1),
				ctr		= vec3(0),
				up		= vec3(0, 1, 0);

		float	fovy	= 45.f,
				near	= .1f,
				far		= 1000.f;
	};
};
