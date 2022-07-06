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
			struct WindowProperties {
				int w = 0;
				int h = 0;
				const char* name = "";
			};

			// Deleted on Creation
			WindowProperties* properties = nullptr;

			unsigned int _vtxs = 0;
			unsigned int _inds = 0;
			unsigned int _texs = 0;
			unsigned int _unfs = 0;
		};

		DEFINE_THREAD(Window,
			WindowData*,
			GLFWwindow* window = nullptr;
			std::vector<Task*> inputs;
			void onInit() override;
			void onKill() override;
			)
		{
			glfwPollEvents();

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
			glGenBuffers(1, &data->_texs);
			glGenBuffers(1, &data->_unfs);

			glBindBuffer(GL_ARRAY_BUFFER,			data->_vtxs);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,	data->_inds);
			glBindBuffer(GL_TEXTURE_BUFFER,			data->_texs);
			glBindBuffer(GL_UNIFORM_BUFFER,			data->_unfs);
		};

		void Window::onKill()
		{
			for (auto& t : kernel)
				delete t;

			delete data;

			glDeleteBuffers(1, &data->_vtxs);
			glDeleteBuffers(1, &data->_inds);
			glDeleteBuffers(1, &data->_texs);
			glDeleteBuffers(1, &data->_unfs);

			glfwDestroyWindow(window);
		};

		std::unique_ptr<Thread> create(int w, int h, const char* name)
		{
			return Window::create(new WindowData{ new WindowData::WindowProperties({w, h, name}), });
		};
		
		struct LoadData
		{
			const char* f1 = nullptr;
			const char* f2 = nullptr;
			const char* f3 = nullptr;
			unsigned int ret = 0;
			bool done = false;
		};
		

		// LOADING SHADERS USING WINDOW
		DEFINE_TASK(LoadShader, LoadData*, )
		{
			if (data->f3)
				data->ret = ShaderManager::create(data->f1, data->f2, data->f3);
			else if (data->f2)
				data->ret = ShaderManager::create(data->f1, data->f2);
			else
				data->ret = ShaderManager::create(data->f1);

			data->done = true;

			delete data;
			data = nullptr;
			delete this;
		};
		unsigned int load_shader(Thread* thread, const char* f1)
		{
			LoadData* data = new LoadData({ f1 });

			thread->push(LoadShader::create(data).release());

			while (!data->done);

			return data->ret;
		};
		unsigned int load_shader(Thread* thread, const char* f1, const char* f2)
		{
			LoadData* data = new LoadData({ f1, f2 });

			thread->push(LoadShader::create(data).release());

			while (!data->done);

			return data->ret;
		};
		unsigned int load_shader(Thread* thread, const char* f1, const char* f2, const char* f3)
		{
			LoadData* data = new LoadData({ f1, f2, f3 });

			thread->push(LoadShader::create(data).release());

			while (!data->done);

			return data->ret;
		};
		// LOADING SHADERS USING WINDOW


		// LOADING TEXTURES USING WINDOW
		DEFINE_TASK(LoadTexture, LoadData*, )
		{
			data->ret = TextureManager::create(data->f1);

			data->done = true;

			delete data;
			data = nullptr;
			delete this;
		}
		unsigned int load_texture(Thread* thread, const char* file_name)
		{
			LoadData* data = new LoadData({ file_name });

			thread->push(LoadTexture::create(data).release());

			while (!data->done);

			return data->ret;
		};
		// LOADING TEXTURES USING WINDOW
	};
};
