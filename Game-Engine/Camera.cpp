#include "Camera.h"

#include "Engine.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

namespace eng
{
	void Camera::update()
	{
		proj = perspective(45.f, 1.f, .1f, 1000.f);
		view = lookAt(vec3(0, 0, 1), vec3(0, 0, 0), vec3(0, 0, 0));
		rotn = roll * yaww * ptch;
		mode = rotn * trns;
		mvp = proj * view * mode;
	};
};
