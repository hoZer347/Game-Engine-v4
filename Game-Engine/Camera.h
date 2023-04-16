#pragma once

#include "Data.h"

#include <glm/glm.hpp>
using namespace glm;

#include <vector>

namespace loom
{
	// Camera, without this, you won't actually render anything
	struct Camera final : public Updateable
	{
		Camera();
		static inline mat4 mvp = mat4(1);

		float	fovy,
				aspect,
				near,
				far;
		
		vec4	eye,
				ctr,
				up;

	protected:
		friend struct Utils;
		void update() override;

	private:
		mat4	view,
				proj;
	};
	//
};
