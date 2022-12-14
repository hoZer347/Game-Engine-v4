#include "Engine.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include "Task.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
using namespace glm;

#include "Shader.hpp"
#include "Texture.hpp"

#include <mutex>

static inline std::mutex create_window_one_at_a_time;

namespace loom
{
	struct Thread
	{
		std::thread thread;
		std::mutex mut;
		bool KILL = true, PAUSE = false;
		std::vector<Task> tasks;
		std::vector<Task> to_add;
		std::vector<Task> onInvk;
		std::vector<Task> onKill;
	};

	static inline std::vector<Thread*> threads;


	void init()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		stbi_set_flip_vertically_on_load(true);
	};


	void open_window(Thread* thread, const char* name, int w, int h)
	{
		GLuint *_vtxs = new GLuint, * _inds = new GLuint;

		thread->onInvk.emplace(thread->onInvk.begin(), [_vtxs, _inds]()
		{
			create_window_one_at_a_time.lock();

			glfwInit();
			GLFWwindow* window = glfwCreateWindow(640, 640, "Window Title", nullptr, nullptr);
			glewExperimental = true;
			glfwMakeContextCurrent(window);
			glewInit();

			create_window_one_at_a_time.unlock();

			glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int w, int h)
			{
				glViewport(0, 0, w, h);
			});

			glEnable(GL_TEXTURE_2D);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glClearColor(.5, .5, .5, 0);

			glGenBuffers(1, _vtxs);
			glGenBuffers(1, _inds);
			glBindBuffer(GL_ARRAY_BUFFER, *_vtxs);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *_inds);

			// TODO: Add customizable vertex data

			glVertexAttribPointer(
				0, 4,
				GL_FLOAT,
				GL_FALSE,
				64,
				(void*)(0 * sizeof(vec4)));

			glVertexAttribPointer(
				1, 4,
				GL_FLOAT,
				GL_FALSE,
				64,
				(void*)(1 * sizeof(vec4)));

			glVertexAttribPointer(
				2, 4,
				GL_FLOAT,
				GL_FALSE,
				64,
				(void*)(2 * sizeof(vec4)));

			glVertexAttribPointer(
				3, 4,
				GL_FLOAT,
				GL_FALSE,
				64,
				(void*)(3 * sizeof(vec4)));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
		});

		assign(thread, [thread]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				if (glfwWindowShouldClose(window))
					thread->KILL = true;

				glfwSwapBuffers(window);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glfwPollEvents();
			};
		});

		assign_on_kill(thread, [_vtxs, _inds]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
				glfwDestroyWindow(window);

			delete _vtxs;
			delete _inds;
		});
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


	void run(std::vector<Thread*> threads)
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
					thread->mut.lock();
					if (!thread->to_add.empty())
					{
						thread->tasks.reserve(thread->to_add.size());
						for (auto& task : thread->to_add)
							thread->tasks.emplace_back(task);
						thread->to_add.clear();
					};
					thread->mut.unlock();

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
			delete thread;
			thread = nullptr;
		};

		threads.clear();

		glfwTerminate();
	};


	void close()
	{
		for (auto& thread : threads)
			thread->KILL = true;
	};


	Thread* create_thread()
	{
		threads.emplace_back(new Thread());

		return threads.back();
	};
};
