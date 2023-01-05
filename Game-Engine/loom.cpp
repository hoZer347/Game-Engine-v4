#include "loom.h"

#include "Shader.hpp"
#include "Texture.hpp"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <unordered_map>
#include <barrier>
#include <mutex>

namespace loom
{
	void Init()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		glfwInit();
		GLFWwindow* window = glfwCreateWindow(640, 640, "Title", nullptr, nullptr);
		glewExperimental = true;
		glfwMakeContextCurrent(window);
		glewInit();

		uint32_t _vtxs, _inds;
		glGenBuffers(1, &_vtxs);
		glGenBuffers(1, &_inds);

		glBindBuffer(GL_ARRAY_BUFFER, _vtxs);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _inds);

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

		glClearColor(.5, .5, .5, 0);

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int w, int h)
		{
			glViewport(0, 0, w, h);
		});

		Attribute<Shader>([](const Shader& shader)
		{
			glUseProgram(shader.data);
		});

		Attribute<Texture>([](const Texture& texture)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_2D, texture.data);
		});

		Attribute<Inds>([](const Inds& inds)
		{
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				inds.data.size() * sizeof(uint32_t),
				inds.data.data(),
				GL_STATIC_DRAW);
		});
		Attribute<Vtxs>([](const Vtxs& vtxs)
		{
			glBufferData(
				GL_ARRAY_BUFFER,
				vtxs.data.size() * sizeof(Vtx),
				vtxs.data.data(),
				GL_STATIC_DRAW);
		});
		Attribute<Draw>([](const Draw& draw)
		{
			glDrawElements(draw.data, 4, GL_UNSIGNED_INT, nullptr);
		});

		glEnable(GL_TEXTURE_2D);
	};

	void Render()
	{
		if (GLFWwindow* window = glfwGetCurrentContext())
		{
			while (!glfwWindowShouldClose(window))
			{
				for (auto& mesh : Mesh::meshes)
				{
					Attribute<Shader>::kernel(View<Shader>(*mesh));
					Attribute<Texture>::kernel(View<Texture>(*mesh));
					Attribute<Inds>::kernel(View<Inds>(*mesh));
					Attribute<Vtxs>::kernel(View<Vtxs>(*mesh));
					Attribute<Draw>::kernel(View<Draw>(*mesh));
				};

				
				glfwPollEvents();
				glfwSwapBuffers(window);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			};
		};
	};

	void Exit()
	{
		glfwTerminate();
	};
};
