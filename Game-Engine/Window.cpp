#include "Window.h"

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
#ifndef WINDOW_IMPL
	struct _Window : public Window
	{
		virtual ~_Window() { };

		void assign(Task task) override;
		void join() override { thread.join(); };
		bool joinable() const noexcept override { return thread.joinable(); };

	protected:
		friend struct Window;
		
		void load_shader(unsigned int& i, std::vector<std::string> file_names) override;
		void load_textures(std::vector<unsigned int>& i, std::vector<std::pair<std::string, unsigned int>> textures) override;

		std::vector<Task> tasks;
		GLFWwindow* window = nullptr;
		std::mutex mut;
		unsigned int _vtxs=0, _inds=0;
		bool KILL = false;
		std::thread thread;
		ShaderManager	s_mgr;
		TextureManager	t_mgr;
	};

	void _Window::assign(Task task)
	{
		mut.lock();
		tasks.emplace_back(task);
		mut.unlock();
	};

	void _Window::load_shader(unsigned int& i, std::vector<std::string> file_names)
	{
		assign([this, file_names, &i]()
			{
				i = s_mgr.create(file_names);
			});
	};

	void _Window::load_textures(std::vector<unsigned int>& i, std::vector<std::pair<std::string, unsigned int>> textures)
	{
		assign([this, textures, &i]()
			{
				i = t_mgr.create(textures);
			});
	};

	Window* Window::create(const char* name, int w, int h)
	{
		_Window* window = new _Window();

		window->thread = std::thread([window]()
		{
			create_window_one_at_a_time.lock();

			glfwInit();
			window->window = glfwCreateWindow(640, 640, "Window Title", nullptr, nullptr);
			glewExperimental = true;
			glfwMakeContextCurrent(window->window);
			glewInit();

			create_window_one_at_a_time.unlock();

			glfwSetWindowSizeCallback(window->window, [](GLFWwindow* window, int w, int h)
				{
					glViewport(0, 0, w, h);
				});

			glEnable(GL_TEXTURE_2D);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glClearColor(.5, .5, .5, 0);

			glGenBuffers(1, &window->_vtxs);
			glGenBuffers(1, &window->_inds);
			glBindBuffer(GL_ARRAY_BUFFER, window->_vtxs);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, window->_inds);

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

			while (!window->KILL)
			{
				if (!window->tasks.empty())
				{
					window->mut.lock();
					for (auto& f : window->tasks)
						f();
					window->mut.unlock();
				};

				if (glfwWindowShouldClose(window->window))
					window->KILL = true;

				glfwPollEvents();

				glfwSwapBuffers(window->window);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			};

			glfwDestroyWindow(window->window);
		});

		return window;
	};
#else
	WINDOW_IMPL
#endif
};
