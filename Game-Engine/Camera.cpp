#include "Camera.h"

#include "Enums.h"

#include "Data.h"

#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>

namespace loom
{
	Camera::Camera()
	{
		eye = vec4(-.5, -.5, 1, 1);
		ctr = vec4(.1, 0, 0, 1);
		up  = vec4(0, 0, 1, 1);

		fovy = 45.f;
		aspect = 1.f;
		near = .1f;
		far = 1000.f;
	};

	void Camera::render()
	{
		if (GLFWwindow* window = glfwGetCurrentContext())
		{
			int w, h;
			glfwGetWindowSize(window, &w, &h);
			aspect = (float)w / (float)h;
			view = lookAt(vec3(eye), vec3(ctr), vec3(up));
			proj = perspective(fovy, aspect, near, far);
			mvp = proj * view;

			for (auto& renderable : Renderable::contents)
				renderable->render();
		};
	};
};
