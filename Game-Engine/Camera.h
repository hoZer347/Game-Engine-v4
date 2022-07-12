#pragma once

#include "glm/glm.hpp"
using namespace glm;

namespace eng
{
	class Thread;
	class Task;
	namespace wnd { class Window; };

	struct Camera
	{
		friend class wnd::Window;

	protected:									// Uniform ID
		static inline mat4	mvp  = mat4(1),		// 0
							mode = mat4(1),		// 1
							view = mat4(1),		// 2
							proj = mat4(1);		// 3

		static inline mat4	rotn = mat4(1),		// 4
							trns = mat4(1),		// 5
							roll = mat4(1),		// 6
							yaww = mat4(1),		// 7
							ptch = mat4(1);		// 8

		static void update();

	private:

	};
};
