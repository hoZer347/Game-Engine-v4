#include "Camera.h"

#include "Enums.h"

#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>

namespace loom
{
	Camera::Camera()
	{
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
	void Camera::update()
	{
		if (GLFWwindow* window = glfwGetCurrentContext())
		{
			int w, h;
			glfwGetWindowSize(window, &w, &h);
			aspect = (float)w / (float)h;
			proj = perspective(fovy, aspect, near, far);

			mvp = proj * view * mode;

			for (auto& renderable : Renderable::renderables)
				renderable->render();
		};
	};
};
