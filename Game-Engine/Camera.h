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

	protected:
		friend struct Loom;
		static inline std::vector<Camera*> cameras;

	private:
		mat4	mode,
				view,
				proj;
		
		vec4	pos;

		vec3	eye,
				ctr,
				up;

		float	fovy=0,
				aspect=0,
				near=0,
				far=0;
	};
	//
};
