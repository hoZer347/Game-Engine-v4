#include "Engine.h"

#include "Enums.h"
#include "Data.h"
#include "Timing.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>

namespace loom
{
	static inline bool is_running = false;
	thread_local static inline bool is_on_main = false, is_on_updater = false;

	void Engine::Init()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	};
	void Engine::Run()
	{
		is_running = true;
		is_on_main = true;


		// TIMER for FPS counting
		Timer TIMER;


		// Preparing openGL
		glfwInit();
		thread_local static GLFWwindow* window = glfwCreateWindow(640, 640, "Title", nullptr, nullptr);
		glewExperimental = true;
		glfwMakeContextCurrent(window);
		glewInit();
		glClearColor(.5, .5, .5, 0);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0);
		glDepthFunc(GL_LESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_MULTISAMPLE);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glEnable(GL_DOUBLEBUFFER);
		glEnable(GL_CULL_FACE);
		glfwSwapInterval(0);
		glEnable(GL_DEBUG_OUTPUT);
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
			{
				std::cout << type << std::endl;
				std::cout << source << std::endl;
				std::cout << id << std::endl;
				std::cout << message << std::endl;
			};
		}, nullptr);
		GLuint _vtxs, _inds;
		glGenBuffers(1, &_vtxs);
		glGenBuffers(1, &_inds);
		glBindBuffer(GL_ARRAY_BUFFER, _vtxs);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _inds);
		glVertexAttribPointer(VEC4_0_16, 4, GL_FLOAT, GL_FALSE, 0, (void*)(0 * sizeof(vec4)));
		glVertexAttribPointer(VEC4_0_32, 4, GL_FLOAT, GL_FALSE, 32, (void*)(0 * sizeof(vec4)));
		glVertexAttribPointer(VEC4_1_32, 4, GL_FLOAT, GL_FALSE, 32, (void*)(1 * sizeof(vec4)));
		glVertexAttribPointer(VEC4_0_64, 4, GL_FLOAT, GL_FALSE, 64, (void*)(0 * sizeof(vec4)));
		glVertexAttribPointer(VEC4_1_64, 4, GL_FLOAT, GL_FALSE, 64, (void*)(1 * sizeof(vec4)));
		glVertexAttribPointer(VEC4_2_64, 4, GL_FLOAT, GL_FALSE, 64, (void*)(2 * sizeof(vec4)));
		glVertexAttribPointer(VEC4_3_64, 4, GL_FLOAT, GL_FALSE, 64, (void*)(3 * sizeof(vec4)));


		// Initializing updater
		std::atomic<bool> KILL = false;
		std::thread updater {[&]()
		{
			is_on_updater = true;
			while (!KILL)
			{
				DoOnUpdater([]() { });
				Updateable::access([](auto& object) { object.update(); });
			};
		}};


		// Main loop
		while (!glfwWindowShouldClose(window))
		{
			// Doing openGL or main-thread-required tasks
			DoOnMain([]() {});


			// Rendering Things
			Renderable::access([](auto& object) { object.render(); });


			// OpenGL Stuff
			glfwSwapBuffers(window);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			// Recording FPS
			// TODO: Once text rendering is done, show on screen
			glfwSetWindowTitle(window, std::to_string(1 / TIMER.GetDiff_s()).c_str());
			TIMER.restart();
		};


		// Deallocating everything allocated that uses openGL
		KILL = true;
		updater.join();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &_vtxs);
		glDeleteBuffers(1, &_inds);
		glfwDestroyWindow(window);
		window = nullptr;
		glfwTerminate();

		is_on_main = true;
		is_running = false;
	};
	const bool& Engine::GetIsRunning()
	{
		return is_running;
	}
	const bool& Engine::GetIsOnMain()
	{
		return is_on_main;
	}
	const bool& Engine::GetIsOnUpdater()
	{
		return is_on_updater;
	};
	void Engine::DoOnMain(Task&& task)
	{
		static std::vector<Task> to_do;
		static std::recursive_mutex mut;

		std::scoped_lock<std::recursive_mutex> lock{mut};
		if (is_running && is_on_main)
		{
			for (auto& task : to_do)
				task();
			task();
			to_do.clear();
			return;
		};

		to_do.emplace_back(task);
	};
	void Engine::DoOnUpdater(Task&& task)
	{
		static std::vector<Task> to_do;
		static std::recursive_mutex mut;

		std::scoped_lock<std::recursive_mutex> lock{mut};
		if (is_running && is_on_updater)
		{
			for (auto& task : to_do)
				task();
			task();
			to_do.clear();
			return;
		};

		to_do.emplace_back(task);
	};
};
