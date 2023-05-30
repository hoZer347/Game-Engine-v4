#pragma once

#include "Data.h"

#include <glm/glm.hpp>
using namespace glm;

#include <vector>

namespace loom
{
	struct Camera final
	{
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

		static inline mat4	trns		= mat4(1),
							rotn		= mat4(1),
							mode		= mat4(1),
							view		= mat4(1),
							proj		= mat4(1),
							roll_mat	= mat4(1),
							ptch_mat	= mat4(1),
							yaww_mat	= mat4(1);

		static inline vec4	mpos, mdir;


		// Initiates "Free Cam"
		// Controls:
		// - Press and hold middle mouse button to rotate camera
		// - WASD movement relative to camera rotation around the Z axis
		// - LEFT_CTRL + SPACE go up and down on the Z axis
		// - ESCAPE exits Free Cam
		static void InitiateFreeCam();


	protected:
		friend struct Loom;
		static void update();

	private:
		Camera() { };
		static inline mat4 _mvp = mat4(1);

	public:
		static inline mat4& mvp = _mvp;
	};
	//
};
