#include "Engine.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include "Task.h"
#include "Data.h"
#include "Buffer.h"

#include "glm/glm.hpp"
using namespace glm;

#include <mutex>
#include <memory>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

static inline std::mutex create_window_one_at_a_time;

namespace loom
{
	struct Thread
	{
		std::thread thread;
		std::mutex mut;
		bool KILL = true, PAUSE = false;
		std::vector<Task> tasks;
		std::vector<Task> singletons;
		std::vector<Task> to_add;
		std::vector<Task> onInvk;
		std::vector<Task> onKill;
	};

	static inline std::unordered_set<Thread*> threads;

	void init()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		//stbi_set_flip_vertically_on_load(true);
	};

	void finish()
	{
		for (auto& thread : threads)
			delete thread;
	};


	void open_window(Thread* thread, const char* name, int w, int h)
	{
		thread->onInvk.emplace(thread->onInvk.begin(), [thread, name, w, h]()
		{
			create_window_one_at_a_time.lock();

			glfwInit();
			GLFWwindow* window = glfwCreateWindow(w, h, name, nullptr, nullptr);
			glewExperimental = true;
			glfwMakeContextCurrent(window);
			glewInit();

			create_window_one_at_a_time.unlock();

			glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int w, int h)
			{
				glViewport(0, 0, w, h);
			});

			glfwSwapInterval(0);

			glEnable(GL_TEXTURE_2D);

			glEnable(GL_DEBUG_OUTPUT);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_MULTISAMPLE);
			glfwWindowHint(GLFW_SAMPLES, 4);

			glClearColor(.5, .5, .5, 0);

			GLuint _vtxs, _inds;

			glGenBuffers(1, &_vtxs);
			glGenBuffers(1, &_inds);
			glBindBuffer(GL_ARRAY_BUFFER,			_vtxs);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,	_inds);

			// TODO: Add customizable vertex data

			glVertexAttribPointer(
				0, 4,
				GL_FLOAT,
				GL_FALSE,
				sizeof(Vtx),
				(void*)(0 * sizeof(vec4)));
			glVertexAttribPointer(
				1, 4,
				GL_FLOAT,
				GL_FALSE,
				sizeof(Vtx),
				(void*)(1 * sizeof(vec4)));
			glVertexAttribPointer(
				2, 4,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)(2 * sizeof(vec4)));
			glVertexAttribPointer(
				3, 4,
				GL_FLOAT,
				GL_FALSE,
				sizeof(Vtx),
				(void*)(3 * sizeof(vec4)));
			
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);

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

			thread->onKill.emplace_back([_vtxs, _inds]()
			{
				if (GLFWwindow* window = glfwGetCurrentContext())
				{
					glfwDestroyWindow(window);

					GLuint __vtxs = _vtxs;
					GLuint __inds = _inds;

					glDeleteBuffers(1, &__vtxs);
					glDeleteBuffers(1, &__inds);
				};
			});
		});

		assign(thread, [thread]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				if (glfwWindowShouldClose(window))
					thread->KILL = true;
					//close();

				glfwSwapBuffers(window);

				std::this_thread::sleep_for(std::chrono::milliseconds(50));

				double timer = glfwGetTime();
				timer = timer * 1000;
				glfwSetTime(0);
				std::string s = "FPS: " + std::to_string((unsigned int)timer);

				glfwSetWindowTitle(window, s.c_str());

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glfwPollEvents();
			};
		});
	};


	void assign_once(Thread* thread, Task task)
	{
		thread->mut.lock();
		thread->singletons.emplace_back(task);
		thread->mut.unlock();
	};
	void assign(Thread* thread, Task task)
	{
		thread->mut.lock();
		thread->to_add.emplace_back(task);
		thread->mut.unlock();
	};
	void assign_on_init(Thread* thread, Task task)
	{
		thread->mut.lock();
		thread->onInvk.emplace_back(task);
		thread->mut.unlock();
	};
	void assign_on_kill(Thread* thread, Task task)
	{
		thread->mut.lock();
		thread->onKill.emplace_back(task);
		thread->mut.unlock();
	};


	void run()
	{
		run(threads);
	};


	void run(std::unordered_set<Thread*> threads)
	{
		for (auto& thread : threads)
			thread->thread = std::thread([thread]()
			{
				thread->mut.lock();
				thread->KILL = false;
				for (auto& task : thread->onInvk)
					task();
				thread->mut.unlock();

				while (!thread->KILL)
				{
					if (!thread->to_add.empty())
					{
						thread->mut.lock();
						thread->tasks.reserve(thread->to_add.size());
						for (auto& task : thread->to_add)
							thread->tasks.emplace_back(task);
						for (auto& task : thread->singletons)
							task();
						thread->singletons.clear();
						thread->to_add.clear();
						thread->mut.unlock();
					};

					for (auto& task : thread->tasks)
						task();
				};

				thread->mut.lock();
				for (auto& task : thread->onKill)
					task();
				thread->mut.unlock();
			});

		for (auto& thread : threads)
		{
			if (thread->thread.joinable())
				thread->thread.join();
		};

		glfwTerminate();
	};


	void close()
	{
		close(threads);
	};


	void close(std::unordered_set<Thread*> threads)
	{
		for (auto& thread : threads)
			thread->KILL = true;
	};


	Thread* create_thread()
	{
		Thread* thread = new Thread();
		threads.emplace(thread);

		return thread;
	};


	void destroy(Thread* thread)
	{
		thread->KILL = true;

		if (thread->thread.joinable())
			thread->thread.join();

		threads.erase(thread);
		delete thread;
		thread = nullptr;
	};
};
