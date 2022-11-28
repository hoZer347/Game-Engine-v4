#include "Engine.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
using namespace glm;

#include "Camera.h"

#include "Shader.hpp"
#include "Texture.hpp"

#include <iostream>

inline std::mutex create_window_one_at_a_time;

namespace eng
{
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Thread::Thread()
	{
		thread = std::thread([this]()
			{
				while (!KILL)
				{
					mut.lock();
					for (auto& task : tasks)
						if (task && !(*task)())
							task = nullptr;
					mut.unlock();
				};
			});
	};

	Thread::~Thread()
	{
		KILL = true;

		if (thread.joinable())
			thread.join();
	};

	void Thread::assign(Task task)
	{
		mut.lock();
		tasks.emplace_back(new Task(task));
		mut.unlock();
	};

	void Thread::modify(std::function<void(std::vector<std::shared_ptr<Task>>&)> vec_func)
	{
		mut.lock();
		vec_func(tasks);
		mut.unlock();
	};

	void Thread::clean()
	{
		assign([this]()
			{
				tasks.erase(std::remove_if(tasks.begin(), tasks.end(), [](std::shared_ptr<Task>& t) { return !(bool)t; }), tasks.end());
				return false;
			});
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	


	////////////////////////////////////////////////////////////////////////////////////////////////////
	
	Window::Window()
	:	Thread(),
		s_mgr(std::make_shared<ShaderManager>()),
		t_mgr(std::make_shared<TextureManager>()),
		camera(std::make_unique<Camera>())
	{
		assign([this]()
			{
				create_window_one_at_a_time.lock();
				
				glfwInit();
				glfw_window = glfwCreateWindow(640, 640, "Window Title", nullptr, nullptr);
				glewExperimental = true;
				glfwMakeContextCurrent(glfw_window);
				glewInit();

				create_window_one_at_a_time.unlock();

				glfwSetWindowSizeCallback(glfw_window, [](GLFWwindow* window, int w, int h)
					{
						glViewport(0, 0, w, h);
					});

				glEnable(GL_TEXTURE_2D);

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glClearColor(.5, .5, .5, 0);

				glGenBuffers(1, &_vtxs);
				glGenBuffers(1, &_inds);
				glBindBuffer(GL_ARRAY_BUFFER, _vtxs);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _inds);

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

				return false;
			});

		assign([this]()
			{
				if (glfw_window)
				{
					// TODO: remove
					if (glfwWindowShouldClose(glfw_window))
						KILL = true;

					glfwPollEvents();

					camera->update();

					glfwSwapBuffers(glfw_window);

					glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
				}

				return true;
			});
	};
	
	Window::~Window()
	{
		glfwDestroyWindow(glfw_window);
	};

	void Window::load_shader(std::vector<std::string> file_names, unsigned int& shader)
	{
		assign([this, file_names, &shader]()
			{
				shader = s_mgr->create(file_names);

				return false;
			});
	};

	void Window::load_textures(std::vector<std::pair<std::string, unsigned int>> file_names, std::vector<unsigned int>& textures)
	{
		assign([this, file_names, &textures]()
			{
				auto vec = t_mgr->create(file_names);

				textures.insert(textures.end(), vec.begin(), vec.end());

				return false;
			});
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////////////////////////////////

	void start()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		stbi_set_flip_vertically_on_load(true);
	};

	void close()
	{
		glfwTerminate();
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
};
