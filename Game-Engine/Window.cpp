#include "Window.h"

#include "Mem_Leak.h"

// OpenGL
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;
//

#include "Engine.h"
#include "Shader.h"
#include "Texture.h"

#include <iostream>

std::mutex windows_one_at_a_time;

DEFINE_THREAD(Window, GLFWwindow,
	GLuint _vtxs = 0;
	GLuint _inds = 0;
	GLuint _texs = 0;
	GLuint _unfs = 0;
	void init() override;
	friend class CreateWindow; )
{
	glfwPollEvents();

	if (glfwWindowShouldClose(o)) KILL = true;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwSwapBuffers(o);

	assign(this);
};

DEFINE_TASK(CreateWindow, Window, )
{
	std::scoped_lock<std::mutex> guard(windows_one_at_a_time);

	glfwInit();
	o->o = glfwCreateWindow(640, 640, "", nullptr, nullptr);
	glfwMakeContextCurrent(o->GetObject());
	glfwSwapInterval(0);
	glewExperimental = true;
	glewInit();

	glGenBuffers(1, &o->_vtxs);
	glGenBuffers(1, &o->_inds);
	glGenBuffers(1, &o->_texs);
	glGenBuffers(1, &o->_unfs);

	glBindBuffer(GL_ARRAY_BUFFER,			o->_vtxs);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,	o->_inds);
	glBindBuffer(GL_TEXTURE_BUFFER,			o->_texs);
	glBindBuffer(GL_UNIFORM_BUFFER,			o->_unfs);

	glfwSetWindowSizeCallback(o->GetObject(), [](GLFWwindow* w, int width, int height)
	{
		glViewport(0, 0, width, height);
	});

	glClearColor(.5, .5, .5, 0);

	delete this;
};

void Window::init()
{
	assign(CreateWindow::create(this).release());

	t = std::thread([this]() {

		// Main thread (same as eng::Thread)
		while (!KILL)
		{
			if (!tasks.empty())
			{
				tasks.front()->exec();
				tasks.pop();
			}

			while (PAUSE) if (KILL) break;
		};

		// Emptying the task queue (same as eng::Thread)
		while (!tasks.empty())
			tasks.pop();

		// Destructor
		if (_vtxs) glDeleteBuffers(1, &_vtxs);
		if (_inds) glDeleteBuffers(1, &_inds);
		if (_texs) glDeleteBuffers(1, &_texs);
		if (_unfs) glDeleteBuffers(1, &_unfs);
		glfwDestroyWindow(o);
		});
};

DEFINE_TASK(AddMesh, Window, )
{
	delete this;
};

namespace eng
{
	namespace wnd
	{
		std::unique_ptr<Thread> create()
		{
			return Window::create(nullptr);
		};
	};
};
