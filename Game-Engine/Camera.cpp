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
		roll = rotate(roll * trns, .01f, vec3(0, 1, 0)) * inverse(trns);

		proj = perspective(45.f, 1.f, .1f, 1000.f);
		view = lookAt(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0));
		rotn = yaww * ptch * roll;
		mode = rotn * trns;
		mvp = proj * view * mode;

		glUniformMatrix4fv(MVP,  1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(MODE, 1, GL_FALSE, &mode[0][0]);
		glUniformMatrix4fv(VIEW, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(PROJ, 1, GL_FALSE, &proj[0][0]);
		glUniformMatrix4fv(ROTN, 1, GL_FALSE, &rotn[0][0]);
		glUniformMatrix4fv(TRNS, 1, GL_FALSE, &trns[0][0]);
		glUniformMatrix4fv(YAWW, 1, GL_FALSE, &roll[0][0]);
		glUniformMatrix4fv(PTCH, 1, GL_FALSE, &yaww[0][0]);
		glUniformMatrix4fv(ROLL, 1, GL_FALSE, &ptch[0][0]);
	};
};
