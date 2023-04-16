#include "Camera.h"

#include "Enums.h"
#include "Utils.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>

namespace loom
{
	Camera::Camera()
	:	eye(0, 0, 10, 1),
		ctr(5, 5, 0, 1),
		up(0, 0, 1, 1),
		fovy(45.f),
		aspect(1.f),
		near(.1f),
		far(1000.f),
		proj(1),
		view(1)
	{ };

	void Camera::update()
	{
		if (GLFWwindow* window = glfwGetCurrentContext())
		{
			int w, h;
			glfwGetWindowSize(window, &w, &h);
			aspect = (float)w / (float)h;
			view = lookAt(vec3(eye), vec3(ctr), vec3(up));
			proj = perspective(fovy, aspect, near, far);
			mvp = proj * view;

			Renderable::render_all();
		};
	};
};
