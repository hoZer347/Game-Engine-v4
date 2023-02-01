#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include <vector>

namespace loom
{
	// Camera, without this, you won't actually render anything
	struct Camera final
	{
		Camera();
		static inline mat4 mvp;

		float	fovy,
				aspect,
				near,
				far;
		
		vec4	eye,
				ctr,
				up;

	protected:
		friend struct Utils;
		void update();

	private:
		mat4	view,
				proj;
	};
	//
};
