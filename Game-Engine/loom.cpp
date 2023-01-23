#include "Loom.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>

#include <mutex>
#include <chrono>
#include <barrier>

#include "Enums.h"
#include "Data.h"
#include "Utils.h"
#include "Shader.h"
#include "Helper.h"
#include "Texture.h"
#include "Camera.h"
#include "Input.h"

#ifndef NUM_BASE_THREADS
#define NUM_BASE_THREADS 256
#endif

namespace loom
{
	static inline GLFWwindow* window = nullptr;
	static inline std::mutex one_at_a_time;
	auto _clock = Clock::now();

	void Loom::Init()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	};
	void Loom::RunOnThisThread()
	{
		// Making sure only 1 of these functions runs at a time
		one_at_a_time.lock();
		//



		// Allowing access to shaders / textures
		s_mgr = new ShaderManager();
		t_mgr = new TextureManager();
		//



		// Opening GLFWwindow
		glfwInit();
		window = glfwCreateWindow(640, 640, "Title", nullptr, nullptr);
		glewExperimental = true;
		glfwMakeContextCurrent(window);
		glewInit();
		//



		// Syncing SyncHelpers
		std::barrier barrier = std::barrier(NUM_BASE_THREADS, []() noexcept
		{
			_clock = Clock::now();
			Input::update();
			for (auto& parallel : Parallel::contents)
				parallel->sync();
		});

		Helper::access([&barrier](Helper* helper) {
			helper->thread = std::thread([helper, &barrier]()
			{
				while (!helper->KILL)
				{
					helper->task();
					barrier.arrive_and_wait();
				};

				barrier.arrive_and_drop();
			});
		});

		for (auto i = NUM_BASE_THREADS; i > Helper::size() + 1; i--)
			barrier.arrive_and_drop();
		//

		
		
		// Loading everything else that needs openGL
		Object::access([](Object* obj) { obj->load(); });
		Input::load();
		//



		// Preparing openGL
		glEnable(GL_DEPTH);
		glEnable(GL_BLEND);
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



		// Main loop
		while (!glfwWindowShouldClose(window))
		{
			barrier.arrive_and_wait();

			glfwSwapBuffers(window);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glfwPollEvents();

			Updatable::access([](Updatable* updatable) { updatable->update(); });
			Camera::access([](Camera* camera) { camera->render(); });

			glfwSetWindowTitle(window, std::to_string((Clock::now() - _clock).count()).c_str());
		};

		Helper::access([](Helper* helper) { helper->kill(); });

		barrier.arrive_and_drop();
		//

		

		// Deallocating everything allocated that uses openGL
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &_vtxs);
		glDeleteBuffers(1, &_inds);
		delete s_mgr;
		delete t_mgr;
		glfwDestroyWindow(window);
		window = nullptr;
		glfwTerminate();
		for (auto& obj : Object::contents)
			obj->unload();
		//



		// Making sure only 1 of these functions runs at a time
		one_at_a_time.unlock();
		//
	};
	void Loom::Exit()
	{

	};
	const double Loom::GetTimeDiff()
	{
		return (double)(Clock::now() - _clock).count();
	};


	void Shader::load()
	{
		id = s_mgr->create(files);
	};
	void Shader::unload()
	{

	};



	void Texture::load()
	{
		id = t_mgr->create(file, type);
	};
	void Texture::unload()
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
