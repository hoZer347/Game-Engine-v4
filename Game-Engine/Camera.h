#pragma once

#include "Data.h"

#include <glm/glm.hpp>
using namespace glm;

#include <vector>

namespace loom
{
	struct Control;
	struct Camera :
		virtual public Renderable
	{
		// Initiates "Free Cam"
		// Controls:
		// - Press and hold middle mouse button to rotate camera
		// - WASD movement relative to camera rotation around the Z axis
		// - LEFT_CTRL + SPACE go up and down on the Z axis
		// - ESCAPE exits Free Cam
		static void InitiateFreeCam(Control& control);

		static inline mat4	vp			= mat4(1),
							mvp			= mat4(1),
							trns		= mat4(1),
							rotn		= mat4(1),
							mode		= mat4(1),
							view		= mat4(1),
							proj		= mat4(1),
							roll_mat	= mat4(1),
							ptch_mat	= mat4(1),
							yaww_mat	= mat4(1);

		static inline float	fovy		= 45.f,
							aspect		= 1.f,
							near		= .1f,
							far			= 1000.f,
							roll		= 0.f,
							ptch		= 0.f,
							yaww		= 0.f;

		static inline vec3	eye			= vec3(0, 0, 1),
							ctr			= vec3(0, 0, 0),
							up			= vec3(0, 1, 0);

		static inline vec4	mpos = vec4(1), mdir = vec4(1);

		static inline int	screen_w = 1, screen_h = 1;

	private:
		void render() override;
	};
	//
};
