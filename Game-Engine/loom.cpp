#include "Loom.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <barrier>
#include <iostream>

#include "Enums.h"
#include "Data.h"
#include "Utils.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Input.h"
#include "Text.h"

namespace loom
{
	void Loom::Init()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		Text<>::Init();
	};
	void Loom::Run()
	{
		// TIMER for FPS counting
		Utils::Timer TIMER;
		//
		


		// Allowing access to shaders / textures
		s_mgr = new ShaderManager();
		t_mgr = new TextureManager();
		//



		// Preparing openGL
		glfwInit();
		thread_local static GLFWwindow* window = glfwCreateWindow(640, 640, "Title", nullptr, nullptr);
		glewExperimental = true;
		glfwMakeContextCurrent(window);
		glewInit();
		glClearColor(.5, .5, .5, 0);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_MULTISAMPLE);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_MULTISAMPLE);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glEnable(GL_DOUBLEBUFFER);
		glfwSwapInterval(0);

		glDebugMessageCallback([](
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
			{
				if (!((int)severity == (int)33387))
				std::cout << message << std::endl;
			}, nullptr);

		GLuint _vtxs, _inds;
		glGenBuffers(1, &_vtxs);
		glGenBuffers(1, &_inds);
		glBindBuffer(GL_ARRAY_BUFFER, _vtxs);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _inds);
		glVertexAttribPointer(VEC4_0_16, 4, GL_FLOAT, GL_FALSE, 0,  (void*)(0 * sizeof(vec4)));
		glVertexAttribPointer(VEC4_0_32, 4, GL_FLOAT, GL_FALSE, 32, (void*)(0 * sizeof(vec4)));
		glVertexAttribPointer(VEC4_1_32, 4, GL_FLOAT, GL_FALSE, 32, (void*)(1 * sizeof(vec4)));
		glVertexAttribPointer(VEC4_0_64, 4, GL_FLOAT, GL_FALSE, 64, (void*)(0 * sizeof(vec4)));
		glVertexAttribPointer(VEC4_1_64, 4, GL_FLOAT, GL_FALSE, 64, (void*)(1 * sizeof(vec4)));
		glVertexAttribPointer(VEC4_2_64, 4, GL_FLOAT, GL_FALSE, 64, (void*)(2 * sizeof(vec4)));
		glVertexAttribPointer(VEC4_3_64, 4, GL_FLOAT, GL_FALSE, 64, (void*)(3 * sizeof(vec4)));

		glfwSetWindowSizeCallback(window, [](GLFWwindow*, int w, int h) { glViewport(0, 0, w, h); });
		//



		// Loading Items
		Input::load();
		//



		// Main loop
		while (!glfwWindowShouldClose(window))
		{
			// Updating Inputs
			Input::update();
			//
			


			// Doing openGL-dependent loads / updates
			Loadable::load_all();
			Updateable::update_all();
			Camera::update();
			//



			// OpenGL Stuff
			glfwSwapBuffers(window);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//


			// Recording FPS
			// TODO: Once text rendering is done, show on screen
			glfwSetWindowTitle(window, std::to_string(1 / TIMER.GetDiff_s()).c_str());
			TIMER.restart();
			//
		};
		//



		// Deallocating everything allocated that uses openGL
		Unloadable::unload_all();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &_vtxs);
		glDeleteBuffers(1, &_inds);
		delete s_mgr;
		delete t_mgr;
		glfwDestroyWindow(window);
		window = nullptr;
		glfwTerminate();
		//
	};
	void Loom::Exit()
	{

	};

	ShaderManager* GetSMgr()
	{
		return s_mgr;
	};
	TextureManager* GetTMgr()
	{
		return t_mgr;
	};
};
