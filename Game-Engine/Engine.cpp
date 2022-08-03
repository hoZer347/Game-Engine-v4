#include "Engine.h"

#include "Mem_Leak.h"

#include <GLFW/glfw3.h>

#include <thread>

#include <iostream>

#include "stb_image.h"

namespace eng
{
	Window* debug_window = nullptr;

	void open(bool debug)
	{
		stbi_set_flip_vertically_on_load(true);

		if (debug)
		{
			debug_window = wnd::open("Debug", 1024, 1024);
		};
	};

	void close()
	{
		if (debug_window)
		{
			wnd::close(debug_window);
		};

		glfwTerminate();
	};
};
