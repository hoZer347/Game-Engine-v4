#include "Camera.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

namespace loom
{
	Task Camera::_update()
	{
		return [this]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
				glfwGetWindowSize(window, &w, &h);

			yaww *= rotate(.01f, up);

			proj = perspective(fovy, (float)w / (float)h, near, far);
			view = lookAt(eye, ctr, up);
			rotn = yaww * ptch * roll;
			mode = rotn * trns;
			mvp = proj * view * mode;

			glUniformMatrix4fv(4, 1, GL_TRUE, value_ptr(mvp));
			//glUniformMatrix4fv(5, 1, GL_FALSE, value_ptr(mode));
			//glUniformMatrix4fv(6, 1, GL_FALSE, value_ptr(view));
			//glUniformMatrix4fv(7, 1, GL_FALSE, value_ptr(proj));
			//glUniformMatrix4fv(8, 1, GL_FALSE, value_ptr(rotn));
			//glUniformMatrix4fv(9, 1, GL_FALSE, value_ptr(trns));
			//glUniformMatrix4fv(10, 1, GL_FALSE, value_ptr(roll));
			//glUniformMatrix4fv(11, 1, GL_FALSE, value_ptr(yaww));
			//glUniformMatrix4fv(12, 1, GL_FALSE, value_ptr(ptch));
		};
	};
};
