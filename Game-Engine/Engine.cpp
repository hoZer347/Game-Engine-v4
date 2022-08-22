#include "Engine.h"

#include "Mem_Leak.h"

#include <GLFW/glfw3.h>

#include "Thread.h"
#include <thread>

#include <iostream>

#include "stb_image.h"

namespace eng
{
	Window* debug_window = nullptr;

	void open(bool debug)
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		
		stbi_set_flip_vertically_on_load(true);

		if (debug)
		{
			debug_window = wnd::open("Debug", 1024, 1024);
		};
	};

	void join()
	{
		while (NUM_THREADS > 1);
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
