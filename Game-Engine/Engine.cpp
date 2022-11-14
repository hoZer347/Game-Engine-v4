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
					
					for (auto& t : tasks)
						if (t && !(*t)())
							t = nullptr;

					mut.unlock();
				};

				for (auto& t : on_kill)
					if (t)
						(*t)();
			});
	};

	Thread::~Thread()
	{
		KILL = true;

		if (thread.joinable())
			thread.join();
	};

	void Thread::assign(std::function<bool()> task)
	{
		mut.lock();
		tasks.push_back(std::make_shared<std::function<bool()>>(task));
		mut.unlock();
	};

	void Thread::assign_on_kill(std::function<bool()> task)
	{
		mut.lock();
		on_kill.push_back(std::make_shared<std::function<bool()>>(task));
		mut.unlock();
	};

	void Thread::assign(Task task)
	{
		mut.lock();
		tasks.push_back(task);
		mut.unlock();
	};

	void Thread::assign_on_kill(Task task)
	{
		mut.lock();
		on_kill.push_back(task);
		mut.unlock();
	};

	void Thread::modify(std::function<void(std::vector<Task>&)> vec_func)
	{
		mut.lock();
		vec_func(tasks);
		mut.unlock();
	};

	void Thread::clean()
	{
		mut.lock();
		tasks.erase(std::remove_if(tasks.begin(), tasks.end(), [](Task t) { return !(bool)t; }), tasks.end());
		mut.unlock();
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
					if (glfwWindowShouldClose(glfw_window))
						KILL = true;

					glfwPollEvents();

					camera->update();

					glfwSwapBuffers(glfw_window);

					glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
				}

				return true;
			});

		assign_on_kill([this]()
			{
				glfwDestroyWindow(glfw_window);

				return false;
			});
	};
	
	Window::~Window()
	{
		
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
		stbi_set_flip_vertically_on_load(true);
	};

	void close()
	{
		Data<Thread>::access([](auto vec)
			{
				for (auto& i : vec)
					i.reset();
			});
		glfwTerminate();
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
};
