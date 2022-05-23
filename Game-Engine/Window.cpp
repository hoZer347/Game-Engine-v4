#include "Window.h"

#include "Mem_Leak.h"

// OpenGL
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;
//

#include "Inputs.h"
#include "Object.h"
#include "Engine.h"

#include <thread>
#include <iostream>

class Window
{
public:
	~Window()
	{
		glfwDestroyWindow(w);
		glfwTerminate();
	}

	static void kernel(Window* w, Engine* e)
	{
		glfwInit();
		w->w = glfwCreateWindow(640, 640, "", NULL, NULL);
		glfwMakeContextCurrent(w->w);
		glfwSwapInterval(0);
		glewExperimental = true;
		glClearColor(.5, .5, .5, 1);
		glewInit();

		while (!eng::get_kill(e))
		{
			inputs::update();

			int W, H;
			glfwGetWindowSize(w->w, &W, &H);
			glViewport(0, 0, W, H);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (glfwWindowShouldClose(w->w)) eng::get_kill(e) = true;

			glfwSwapBuffers(w->w);
		};
	};

	std::thread t;
	GLFWwindow* w = nullptr;
};

namespace win
{
	Window* create(Engine* e)
	{
		Window* w = new Window();

		w->t = std::thread(Window::kernel, w, e);

		return w;
	};

	void del(Window* w)
	{
		w->t.join();

		delete w;
	};
};
