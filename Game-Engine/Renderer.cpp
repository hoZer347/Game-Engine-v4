#include "Engine.h"

// OpenGL
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;
//

#include "Texture.hpp"
#include "Shader.hpp"

#include <iostream>

std::mutex create_window_one_at_a_time;

namespace eng
{
	bool Renderer::test_for_window()
	{
		return glfwGetCurrentContext();
	};

	void Renderer::setup(const char* title, int width, int height)
	{
		create_window_one_at_a_time.lock();
		glfwInit();
		GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glewExperimental = true;
		glfwSwapInterval(0);
		glewInit();
		create_window_one_at_a_time.unlock();

		glEnable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClearColor(.5, .5, .5, 0);

		glfwSetWindowSizeCallback(window, [](GLFWwindow*, int width, int height)
			{
				glViewport(0, 0, width, height);
			});

		unsigned int VBO=0, EBO=0;
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

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
	};

	void Renderer::update()
	{
		if (GLFWwindow* window = glfwGetCurrentContext()) {
			glfwPollEvents();
			glfwSwapBuffers(window);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else
		{
			std::cout << "Window not initialized" << std::endl;
		}
	};

	void Renderer::render_this(Material& mat, std::vector<Mesh>& m)
	{
		GLFWwindow* window = glfwGetCurrentContext();

		if (!window) return;

		glUseProgram(mat.shader);

		for (auto& t : mat.texs)
			glBindTexture(GL_TEXTURE_2D, t);

		for (auto& mesh : m)
		{
			glBufferData(
				GL_ARRAY_BUFFER,
				mesh.vtxs.size() * sizeof(Vtx),
				mesh.vtxs.data(),
				GL_DYNAMIC_DRAW);

			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				mesh.inds.size() * sizeof(unsigned int),
				mesh.inds.data(),
				GL_STATIC_DRAW);

			glDrawElements(mat.draw_mode, (GLsizei)mesh.inds.size(), GL_UNSIGNED_INT, nullptr);
		};

		for (auto& t : mat.texs)
			glBindTexture(GL_TEXTURE_2D, 0);
	};

	unsigned int Renderer::load_shader(const char* f1)
	{
		return ShaderManager::create(f1);
	};
	unsigned int Renderer::load_shader(const char* f1, const char* f2)
	{
		return ShaderManager::create(f1, f2);
	};
	unsigned int Renderer::load_shader(const char* f1, const char* f2, const char* f3)
	{
		return ShaderManager::create(f1, f2, f3);
	};
	unsigned int Renderer::load_texture(const char* file_name, unsigned int type)
	{
		return TextureManager::create(file_name, type);
	};
};
