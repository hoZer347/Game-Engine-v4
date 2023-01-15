#include "Loom.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>

#include <mutex>
#include <shared_mutex>

#include "Enums.h"
#include "Data.h"
#include "Helper.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

namespace loom
{
	static inline GLFWwindow* window = nullptr;
	static inline std::mutex create_windows_one_at_a_time;
	static inline std::shared_mutex window_is_rendering;
	std::atomic<bool> isRunning = false;

	void Loom::Init()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	};
	void Loom::RunOnThisThread()
	{
		create_windows_one_at_a_time.lock();

		s_mgr = new ShaderManager();
		t_mgr = new TextureManager();

		glfwInit();
		window = glfwCreateWindow(640, 640, "Title", nullptr, nullptr);
		glewExperimental = true;
		glfwMakeContextCurrent(window);
		glewInit();

		for (auto& obj : Object::objects)
			obj->load();

		create_windows_one_at_a_time.unlock();

		isRunning = true;

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
		glBindBuffer(GL_ARRAY_BUFFER, _inds);

		glVertexAttribPointer(VEC4_0_16, 4, GL_FLOAT, GL_FALSE, 0, (void*)(0 * sizeof(vec4)));

		glVertexAttribPointer(VEC4_0_32, 4, GL_FLOAT, GL_FALSE, 32, (void*)(0 * sizeof(vec4)));
		glVertexAttribPointer(VEC4_1_32, 4, GL_FLOAT, GL_FALSE, 32, (void*)(1 * sizeof(vec4)));

		glVertexAttribPointer(VEC4_0_64, 4, GL_FLOAT, GL_FALSE, 64, (void*)(0 * sizeof(vec4)));
		glVertexAttribPointer(VEC4_1_64, 4, GL_FLOAT, GL_FALSE, 64, (void*)(1 * sizeof(vec4)));
		glVertexAttribPointer(VEC4_2_64, 4, GL_FLOAT, GL_FALSE, 64, (void*)(2 * sizeof(vec4)));
		glVertexAttribPointer(VEC4_3_64, 4, GL_FLOAT, GL_FALSE, 64, (void*)(3 * sizeof(vec4)));

		glfwSetWindowSizeCallback(window, [](GLFWwindow*, int w, int h) { glViewport(0, 0, w, h); });

		while (!glfwWindowShouldClose(window))
		{
			glfwSwapBuffers(window);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glfwPollEvents();

			for (auto& obj : Updatable::updatables)
				obj->update();

			window_is_rendering.lock();

			for (auto& cam : Camera::cameras)
				cam->render_all();

			window_is_rendering.unlock();
		};

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDeleteBuffers(1, &_vtxs);
		glDeleteBuffers(1, &_inds);

		for (auto& obj : Object::objects)
			obj->unload();

		delete s_mgr;
		delete t_mgr;

		glfwDestroyWindow(window);

		glfwTerminate();
	};
	void Loom::Exit()
	{

	};



	void Shader::load()
	{
		id = s_mgr->create(files);
		glUseProgram(id);
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



	bool Loom::IsRunning()
	{
		return isRunning;
	};
	void Loom::lock()
	{
		window_is_rendering.lock_shared();
	};
	void Loom::unlock()
	{
		window_is_rendering.unlock_shared();
	};
	void Loom::Add(Object* obj)
	{
		
	};
	void Loom::Add(Renderable* obj)
	{

	};
	void Loom::Add(Updatable* obj)
	{

	};
	void Loom::Rmv(Object* obj)
	{

	};
	void Loom::Rmv(Renderable* obj)
	{

	};
	void Loom::Rmv(Updatable* obj)
	{

	};
};
