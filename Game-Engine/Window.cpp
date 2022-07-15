#include "Window.h"

#include "Mem_Leak.h"

// OpenGL
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;
//

#include "Engine.h"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.h"

#include <atomic>
#include <vector>
#include <map>

#include <iostream>

namespace eng
{
	namespace wnd
	{
		std::mutex create_window_one_at_a_time;

		struct WindowData
		{
			struct WindowProperties
			{
				int w = 0;
				int h = 0;
				const char* name = "";
			};

			// Deleted on Creation
			WindowProperties* properties = nullptr;

			unsigned int _vtxs = 0,
						 _inds = 0,
						 _texs = 0,
						 _unfs = 0;
		};

		DEFINE_THREAD(Window,
			WindowData,
			GLFWwindow* window = nullptr;
			std::vector<Task*> inputs;
			void onInit() override;
			void onKill() override;
			)
		{
			glfwPollEvents();

			Camera::update();

			if (glfwWindowShouldClose(window)) KILL = true;

			glfwSwapBuffers(window);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		};

		void Window::onInit()
		{
			std::scoped_lock<std::mutex> guard(create_window_one_at_a_time);

			glfwInit();
			window = glfwCreateWindow(
				data->properties->w,
				data->properties->h,
				data->properties->name,
				nullptr, nullptr);
			glewExperimental = true;
			glfwSwapInterval(0);
			glfwMakeContextCurrent(window);
			glewInit();

			glEnable(GL_TEXTURE_2D);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glfwSetWindowSizeCallback(window, [](GLFWwindow*, int w, int h)
				{
					glViewport(0, 0, w, h);
				});

			glClearColor(.5, .5, .5, 0);

			delete data->properties;
			data->properties = nullptr;

			glGenBuffers(1, &data->_vtxs);
			glGenBuffers(1, &data->_inds);

			glBindBuffer(GL_ARRAY_BUFFER,			data->_vtxs);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,	data->_inds);
		};

		void Window::onKill()
		{
			for (auto& t : kernel)
				delete t;

			delete data;

			glDeleteBuffers(1, &data->_vtxs);
			glDeleteBuffers(1, &data->_inds);

			glfwDestroyWindow(window);
		};

		std::unique_ptr<Thread> create(int w, int h, const char* name)
		{
			return Window::create(new WindowData{ new WindowData::WindowProperties({w, h, name}), });
		};
		
		struct LoadData
		{
			unsigned int& ret;
			const char* f1 = nullptr;
			const char* f2 = nullptr;
			const char* f3 = nullptr;
			unsigned int type = 0;
		};

		// LOADING SHADERS USING WINDOW
		DEFINE_TASK(LoadShader, LoadData, )
		{
			if (data->f3)
				data->ret = ShaderManager::create(data->f1, data->f2, data->f3);
			else if (data->f2)
				data->ret = ShaderManager::create(data->f1, data->f2);
			else
				data->ret = ShaderManager::create(data->f1);

			delete data;
			data = nullptr;
			delete this;
		};
		void load_shader(unsigned int& shader, Thread* thread, const char* f1)
		{
			LoadData* data = new LoadData({ shader, f1 });

			thread->push(LoadShader::create(data).release());
		};
		void load_shader(unsigned int& shader, Thread* thread, const char* f1, const char* f2)
		{
			LoadData* data = new LoadData({ shader, f1, f2 });

			thread->push(LoadShader::create(data).release());
		};
		void load_shader(unsigned int& shader, Thread* thread, const char* f1, const char* f2, const char* f3)
		{
			LoadData* data = new LoadData({ shader, f1, f2, f3 });

			thread->push(LoadShader::create(data).release());
		};
		// LOADING SHADERS USING WINDOW


		// LOADING TEXTURES USING WINDOW
		DEFINE_TASK(LoadTexture, LoadData, )
		{
			data->ret = TextureManager::create(data->f1, data->type);

			delete data;
			data = nullptr;
			delete this;
		}
		void load_texture(unsigned int& texture, Thread* thread, const char* file_name, unsigned int type)
		{
			LoadData* data = new LoadData({ texture, file_name });
			data->type = type;

			thread->push(LoadTexture::create(data).release());
		};
		// LOADING TEXTURES USING WINDOW
	};
};
