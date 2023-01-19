#include "Camera.h"

#include "Enums.h"

#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>

namespace loom
{
	Camera::Camera()
	{
		cameras.push_back(this);

		eye = vec3(0, 0, 1);
		ctr = vec3(0, 0, 0);
		up  = vec3(0, 1, 0);

		fovy = 90.f;
		aspect = 1.f;
		near = .1f;
		far = 1000.f;

		mode = mat4(1);
		view = lookAt(eye, ctr, up);
	};
};
