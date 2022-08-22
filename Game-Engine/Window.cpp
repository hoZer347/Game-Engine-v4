#include "Engine.h"

#include "Mem_Leak.h"

#include "Data.h"
#include "Thread.h"
#include "Camera.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <queue>
#include <vector>

#include <mutex>
#include <thread>

#include "Shader.hpp"
#include "Texture.hpp"

std::mutex create_window_one_at_a_time;
std::mutex mut;

namespace eng
{
	struct Window : public Thread
	{
		Window(const char* title, int width, int height) : Thread()
		{
			NUM_THREADS++;

			push([this, title, width, height]()
				{
					create_window_one_at_a_time.lock();

					glfwInit();
					window = glfwCreateWindow(width, height, title, nullptr, nullptr);
					glfwMakeContextCurrent(window);
					glewExperimental = true;
					glfwSwapInterval(0);
					glewInit();

					create_window_one_at_a_time.unlock();

					glfwSetWindowSizeCallback(window, [](GLFWwindow*, int width, int height)
						{
							glViewport(0, 0, width, height);
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
						sizeof(Vtx),
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

					push([this]()
						{
							if (GLFWwindow* window = glfwGetCurrentContext())
							{
								glfwSwapBuffers(window);

								glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

								cam.update();

								if (glfwWindowShouldClose(window))
									KILL = true;

								for (auto& r : render_objs)
								{
									glUseProgram(r->shader);

									for (auto& tex : r->textures)
										glBindTexture(GL_TEXTURE_2D, tex);

									glBufferData(
										GL_ARRAY_BUFFER,
										r->mesh->vtxs.size() * sizeof(Vtx),
										r->mesh->vtxs.data(),
										GL_DYNAMIC_DRAW);

									glBufferData(
										GL_ELEMENT_ARRAY_BUFFER,
										r->mesh->inds.size() * sizeof(unsigned int),
										r->mesh->inds.data(),
										GL_STATIC_DRAW);

									glDrawElements(r->draw_mode, (GLsizei)r->mesh->inds.size(), GL_UNSIGNED_INT, nullptr);

									for (auto& tex : r->textures)
										glBindTexture(GL_TEXTURE_2D, 0);
								};

								glfwPollEvents();
							};

							return true;
						});

					return false;
				});
		};

		void load(RenderObj*& r, Matl& matl, Mesh& mesh, unsigned int draw_mode = 0x0007)
		{
			r = new RenderObj(mesh, draw_mode);
			r->shader = shader_manager.create(matl.shader_files);
			r->textures = texture_manager.create(matl.texture_files);
			render_objs.push_back(r);
		};

		unsigned int _vtxs=0, _inds=0;

		Camera cam;

		ShaderManager shader_manager;
		TextureManager texture_manager;

		std::vector<RenderObj*> render_objs;

		GLFWwindow* window = nullptr;
	};

	namespace wnd
	{
		Window* open(const char* title, int width, int height)
		{
			return new Window(title, width, height);
		};

		void close(Window* window)
		{
			delete window;
			window = nullptr;
		};

		Camera& get_cam(Window* window)
		{
			return window->cam;
		};

		void bind(RenderObj*& render_obj, Window* window, Matl& matl, Mesh& mesh, unsigned int draw_mode)
		{
			window->push([&render_obj, window, &matl, &mesh, draw_mode]()
				{
					window->load(render_obj, matl, mesh, draw_mode);

					return false;
				});

			while (!render_obj);
		};
	};
};
