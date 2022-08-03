#include "Engine.h"

#include "Mem_Leak.h"

#include "Data.h"
#include "Thread.h"

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
	struct RenderObj
	{
		RenderObj(Mesh& mesh, Matl& matl, unsigned int draw_mode) :
			mesh(mesh), draw_mode(draw_mode)
		{
			shader = ShaderManager::create(matl.shader_files);
			textures = TextureManager::create(matl.texture_files);
		};

		~RenderObj() { };

		unsigned int shader, draw_mode;
		std::vector<unsigned int> textures;
		Mesh& mesh;
	};

	struct Window : public Thread
	{
		Window(const char* title, int width, int height) : Thread()
		{
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

								mut.lock();
								for (auto& o : render_objs)
									if (o)
										{
											glUseProgram(o->shader);

											for (auto& tex : o->textures)
												glBindTexture(GL_TEXTURE_2D, tex);

											glBufferData(
												GL_ARRAY_BUFFER,
												o->mesh.vtxs.size() * sizeof(Vtx),
												o->mesh.vtxs.data(),
												GL_DYNAMIC_DRAW);

											glBufferData(
												GL_ELEMENT_ARRAY_BUFFER,
												o->mesh.inds.size() * sizeof(unsigned int),
												o->mesh.inds.data(),
												GL_STATIC_DRAW);

											glDrawElements(o->draw_mode, (GLsizei)o->mesh.inds.size(), GL_UNSIGNED_INT, nullptr);

											for (auto& tex : o->textures)
												glBindTexture(GL_TEXTURE_2D, 0);
										};
								mut.unlock();

								glfwPollEvents();
							}

							return true;
						});

					return false;
				});
		};

		void on_kill()
		{
			glDeleteBuffers(1, &_vtxs);
			glDeleteBuffers(1, &_inds);

			glfwDestroyWindow(window);
		};

		unsigned int _vtxs=0, _inds=0;

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

		void bind(Window* window, Matl& matl, Mesh& mesh, unsigned int draw_mode)
		{
			RenderObj* _r;

			window->push([&_r, window, &matl, &mesh, draw_mode]()
			{
				RenderObj* r = new RenderObj(mesh, matl, draw_mode);

				_r = r;

				window->render_objs.push_back(r);

				return false;
			});
		};
	};
};
