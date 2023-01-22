#pragma once

#include "Data.h"

#include <glm/glm.hpp>
using namespace glm;

#include <vector>

namespace loom
{
	// Camera, without this, you won't actually render anything
	struct Camera final : public Manage<Camera>
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
		friend struct Loom;
		void render();

	private:
		mat4	view,
				proj;
	};
	//
};
