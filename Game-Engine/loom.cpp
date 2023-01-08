#include "loom.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define STOP_STRUCT_PRIVACY

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Data.h"
#include "Window.h"

namespace loom
{
	std::mutex create_windows_one_at_a_time;

	static inline std::vector<std::thread> threads;

	void Init() noexcept
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	};

	void Run() noexcept
	{
		
	};

	void Exit() noexcept
	{
		glfwTerminate();
	};

	RESULT load(Window& window, Mesh& mesh)
	{
		return 1;
	};

	RESULT remv(Window& window, Mesh& mesh)
	{
		return 1;
	};

	RESULT open(Window& window, int width, int height, const char* title)
	{
		threads.emplace_back([window, width, height, title]
		{
			create_windows_one_at_a_time.lock();

			glfwInit();
			GLFWwindow* _window = glfwCreateWindow(width, height, title, nullptr, nullptr);
			glewExperimental = true;
			glfwMakeContextCurrent(_window);
			glewInit();

			create_windows_one_at_a_time.unlock();

			uint32_t _vtxs = 0, _inds = 0;
			glGenBuffers(1, &_vtxs);
			glGenBuffers(1, &_inds);
			glBindBuffer(GL_ARRAY_BUFFER, _vtxs);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _inds);

			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 64, (void*)(0 * 16));
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 64, (void*)(2 * 16));
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 64, (void*)(1 * 16));
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 64, (void*)(3 * 16));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);

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

				glUseProgram(shader.id);

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
