#include "loom.h"

#include "Shader.hpp"
#include "Texture.hpp"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <unordered_map>
#include <barrier>
#include <mutex>

std::mutex create_window_one_at_a_time;

namespace loom
{
	typedef std::unordered_map<GLFWwindow*,
				std::unordered_map<Shader,
					std::unordered_map<Texture,
						std::unordered_map<DrawMode,
							Mesh*>>>> Storage;

	std::unordered_map<GLFWwindow*, ShaderManager*> s_mgrs;
	std::unordered_map<GLFWwindow*, TextureManager*> t_mgrs;

	Storage _mutable, _immutable;

	std::vector<std::tuple<GLFWwindow*, Shader, Texture, DrawMode, Mesh*>> _toAdd;

	void init()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	};
	void exit()
	{
		for (auto& i : s_mgrs)
			delete i.second;

		for (auto& i : t_mgrs)
			delete i.second;

		_mutable.clear();
		_immutable.clear();
		_toAdd.clear();
	};


	void open_window(Thread& thread, int w, int h, const char* name)
	{
		assign_on_init(thread, [w, h, name, &thread]()
		{
			create_window_one_at_a_time.lock();

			glfwInit();
			GLFWwindow* window = glfwCreateWindow(w, h, name, nullptr, nullptr);
			glewExperimental = true;
			glfwMakeContextCurrent(window);
			glewInit();

			create_window_one_at_a_time.unlock();

			glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int w, int h)
			{
				glViewport(0, 0, w, h);
			});

			glfwSwapInterval(0);

			glEnable(GL_TEXTURE_2D);

			glEnable(GL_DEBUG_OUTPUT);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_MULTISAMPLE);
			glfwWindowHint(GLFW_SAMPLES, 4);

			glClearColor(.5, .5, .5, 0);

			GLuint _vtxs, _inds;

			glGenBuffers(1, &_vtxs);
			glGenBuffers(1, &_inds);
			glBindBuffer(GL_ARRAY_BUFFER,			_vtxs);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,	_inds);

			// TODO: Add customizable vertex data

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
				0,
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

			thread.on_kill.emplace_back([_vtxs, _inds]()
			{
				if (GLFWwindow* window = glfwGetCurrentContext())
				{
					glfwDestroyWindow(window);

					GLuint __vtxs = _vtxs;
					GLuint __inds = _inds;

					glDeleteBuffers(1, &__vtxs);
					glDeleteBuffers(1, &__inds);
				};
			});
		});

		assign(thread, [&thread]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				if (glfwWindowShouldClose(window))
					thread.KILL = true;
					//close();

				for (auto& shader : _immutable[window])
				{
					glUseProgram(shader.first);
					for (auto& texture : shader.second)
					{
						glBindTexture(GL_TEXTURE_2D, texture.first);
						for (auto& draw_mode : texture.second)
						{
							glBufferData(
								GL_ARRAY_BUFFER,
								draw_mode.second->vtxs.size() * sizeof(Vtx),
								draw_mode.second->vtxs.data(),
								GL_DYNAMIC_DRAW);

							glBufferData(
								GL_ELEMENT_ARRAY_BUFFER,
								draw_mode.second->inds.size() * sizeof(Ind),
								draw_mode.second->inds.data(),
								GL_DYNAMIC_DRAW);

							glDrawElements(draw_mode.first, (GLsizei)draw_mode.second->inds.size(), GL_UNSIGNED_INT, nullptr);
						};
						glBindTexture(GL_TEXTURE_2D, texture.first);
					};
				};

				glfwSwapBuffers(window);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glfwPollEvents();
			};
		});
	};


	void assign(Thread& thread,	Task task)
	{
		thread.on_exec.emplace_back(task);
	};
	void assign_once(	Thread& thread,	Task task)
	{
		thread.on_once.emplace_back(task);
	};
	void assign_on_init(Thread& thread,	Task task)
	{
		thread.on_init.emplace_back(task);
	};
	void assign_on_kill(Thread& thread, Task task)
	{
		thread.on_kill.emplace_back(task);
	};


	void run(Threads threads)
	{
		std::barrier bar = std::barrier(MAX_THREADS, []() noexcept
		{
			_mutable.swap(_immutable);
		
			if (!_toAdd.empty())
			{
				_mutable.reserve(_toAdd.size());
				_immutable.reserve(_toAdd.size());

				for (auto& element : _toAdd)
					_mutable  [std::get<0>(element)][std::get<1>(element)][std::get<2>(element)][std::get<3>(element)] = std::get<4>(element);

				for (auto& element : _toAdd)
					_immutable[std::get<0>(element)][std::get<1>(element)][std::get<2>(element)][std::get<3>(element)] = std::get<4>(element);

				_toAdd.clear();
			};
		});

		for (auto i = 0; i < MAX_THREADS - threads.size(); i++)
			bar.arrive_and_drop();

		for (auto& thread : threads)
			thread->on_exec.emplace_back([&bar]() { bar.arrive_and_wait(); });

		for (auto& thread : threads)
			thread->thread = std::thread([thread]()
			{
				for (auto& task : thread->on_init)
					task();

				while (!thread->KILL)
				{
					for (auto& task : thread->on_exec)
						task();

					for (auto& task : thread->on_once)
						task();

					thread->on_once.clear();
				};

				for (auto& task : thread->on_kill)
					task();
			});

		for(auto& thread : threads)
			if (thread->thread.joinable())
				thread->thread.join();

		glfwTerminate();
	};


	void load(
		Shader& shader,
		Thread& window,
		std::string name,
		std::vector<std::string> files)
	{
		assign_once(window, [&shader, name, files]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				// TODO: Add Debugging

				if (!s_mgrs[window])
					s_mgrs[window] = new ShaderManager();

				shader = s_mgrs[window]->create(files);
			};
		});
	};


	void load(
		Texture& texture,
		Thread& window,
		Shader& shader,
		std::string name,
		std::string file,
		TYPE type)
	{
		assign_once(window, [&texture, &shader, file, type]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				// TODO: Add Debugging

				if (!shader)
					return;

				glUseProgram(shader);

				if (!t_mgrs[window])
					t_mgrs[window] = new TextureManager();

				texture = t_mgrs[window]->create(file, type);
			};
		});
	};


	void load(
		Mesh& mesh,
		Thread& loader,
		Shader& shader,
		Texture& texture,
		DrawMode& draw_mode)
	{
		assign_once(loader, [&mesh, &shader, &texture, &draw_mode]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				// TODO: Add Debugging

				if (!shader || !texture || !draw_mode)
					return;

				_toAdd.emplace_back(window, shader, texture, draw_mode, &mesh);
			};
		});
	};
};
