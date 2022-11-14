#include "Camera.h"

#include "Engine.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace eng
{
	void Camera::update()
	{
		GLFWwindow* window = glfwGetCurrentContext();

		int w, h;
		glfwGetWindowSize(window, &w, &h);

		rotn = roll * ptch * yaww;
		proj = perspective(fovy, (float)w / (float)h, near, far);
		view = lookAt(eye, ctr, up);
		mode = trns * rotn;
		mvp = proj * view * mode;

		glUniformMatrix4fv(0, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(1, 1, GL_FALSE, &mode[0][0]);
		glUniformMatrix4fv(2, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(3, 1, GL_FALSE, &proj[0][0]);
		glUniformMatrix4fv(4, 1, GL_FALSE, &rotn[0][0]);
		glUniformMatrix4fv(5, 1, GL_FALSE, &trns[0][0]);
		glUniformMatrix4fv(6, 1, GL_FALSE, &roll[0][0]);
		glUniformMatrix4fv(7, 1, GL_FALSE, &ptch[0][0]);
		glUniformMatrix4fv(8, 1, GL_FALSE, &yaww[0][0]);
	};
};
