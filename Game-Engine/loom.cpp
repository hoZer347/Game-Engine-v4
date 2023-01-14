#include "Loom.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>

#include <mutex>

#include "Enums.h"
#include "Data.h"
#include "Geometry.h"

#include "Shader.h"
#include "Texture.h"

namespace loom
{
	static inline GLFWwindow* window = nullptr;
	static inline std::mutex create_windows_one_at_a_time;

	static inline std::atomic<bool> isRunning = false;

	void Loom::Init()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	};
	void Loom::RunOnThisThread()
	{
		create_windows_one_at_a_time.lock();

		s_mgr = new ShaderManager();
		t_mgr = new TextureManager();

		glfwInit();
		window = glfwCreateWindow(640, 640, "Title", nullptr, nullptr);
		glewExperimental = true;
		glfwMakeContextCurrent(window);
		glewInit();

		for (auto& obj : Object::objects)
			obj->load();

		create_windows_one_at_a_time.unlock();

		isRunning = true;

		glfwSwapInterval(0);
		glEnable(GL_DEPTH);

		glEnable(GL_DEPTH);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_MULTISAMPLE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		glEnable(GL_DEBUG_OUTPUT);

		glEnable(GL_TEXTURE_2D);

		glEnable(GL_MULTISAMPLE);
		glfwWindowHint(GLFW_SAMPLES, 4);

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

			Shader shader{ "shaders/default" };
			Textures textures{ "Resources/stone.png" };
			Draw draw{ GL_QUADS };
			Buffer<64, float> vtxs{ GL_ARRAY_BUFFER, GL_STATIC_DRAW };
			vtxs =
			{
				0, 0, 0, 1,   1, 0, 0, 1,   1, 1, 1, 1,   0, 0, 0, 0,
				1, 0, 0, 1,   1, 0, 0, 1,   1, 1, 1, 1,   1, 0, 0, 0,
				1, 1, 0, 1,   1, 0, 0, 1,   1, 1, 1, 1,   1, 1, 0, 0,
				0, 1, 0, 1,   1, 0, 0, 1,   1, 1, 1, 1,   0, 1, 0, 0,
			};
			Buffer<4, uint32_t> inds{ GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW };
			inds = { 0, 1, 2, 3, };

			Mesh mesh{ shader, textures, draw, vtxs };
			mesh.load();

			while (!glfwWindowShouldClose(_window))
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


				for (auto& id : textures.ids)
					glBindTexture(GL_TEXTURE_2D, id);

				glBufferData(
					vtxs.array_type,
					vtxs.size,
					vtxs.ptr,
					vtxs.render_type);

				glBufferData(
					inds.array_type,
					inds.size,
					inds.ptr,
					inds.render_type);

				glDrawElements(draw.type, 4, GL_UNSIGNED_INT, nullptr);
				
				glfwPollEvents();
				glfwSwapBuffers(_window);

				glBindTexture(GL_TEXTURE_2D, 0);
			};
		});

		threads.back().join();

		return 1;
	};
};
