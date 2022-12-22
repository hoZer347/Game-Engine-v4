#include "Engine.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include "Task.h"
#include "Data.h"
#include "Barrier.h"
#include "Buffer.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
using namespace glm;

#include "Shader.hpp"
#include "Texture.hpp"

#include <unordered_map>

#include <mutex>
#include <memory>

static inline std::mutex create_window_one_at_a_time;

namespace loom
{
	struct Thread
	{
		std::thread thread;
		std::mutex mut;
		bool KILL = true, PAUSE = false;
		std::vector<Task> tasks;
		std::vector<Task> singletons;
		std::vector<Task> to_add;
		std::vector<Task> onInvk;
		std::vector<Task> onKill;
	};

	static inline std::unordered_set<Thread*> threads;
	std::unordered_map<GLFWwindow*, ShaderManager*> window_to_shader;
	std::unordered_map<GLFWwindow*, TextureManager*> window_to_texture;

	struct Deallocator
	{
		virtual ~Deallocator()
		{
			for (auto& thread : threads) delete thread;
			for (auto& i : window_to_shader) delete i.second;
			for (auto& i : window_to_texture) delete i.second;
		};
	};

	static inline Deallocator thread_deallocator;


	void init()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		stbi_set_flip_vertically_on_load(true);
	};


	void open_window(Thread* thread, const char* name, int w, int h)
	{
		GLuint *_vtxs, *_inds;

		thread->onInvk.emplace(thread->onInvk.begin(), [&_vtxs, &_inds, name, w, h]()
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

			//glfwSwapInterval(0);

			glEnable(GL_TEXTURE_2D);

			glEnable(GL_DEBUG_OUTPUT);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_MULTISAMPLE);
			glfwWindowHint(GLFW_SAMPLES, 4);

			glClearColor(.5, .5, .5, 0);

			_vtxs = new GLuint;
			_inds = new GLuint;

			glGenBuffers(1, _vtxs);
			glGenBuffers(1, _inds);
			glBindBuffer(GL_ARRAY_BUFFER,			*_vtxs);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,	*_inds);

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
					if (!severity == 33387)
						std::cout << message << std::endl;
			}, nullptr);
		});

		assign(thread, [thread]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				if (glfwWindowShouldClose(window))
					//thread->KILL = true;
					close();

				glfwSwapBuffers(window);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glfwPollEvents();
			};
		});

		assign_on_kill(thread, [&_vtxs, &_inds]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
				glfwDestroyWindow(window);

			delete _vtxs;
			delete _inds;
		});
	};


	void assign(Task task)
	{
		// TODO:
	};
	void assign_once(Thread* thread, Task task)
	{
		thread->mut.lock();
		thread->singletons.emplace_back(task);
		thread->mut.unlock();
	};
	void assign(Thread* thread, Task task)
	{
		thread->mut.lock();
		thread->to_add.emplace_back(task);
		thread->mut.unlock();
	};
	void assign_on_init(Thread* thread, Task task)
	{
		thread->mut.lock();
		thread->onInvk.emplace_back(task);
		thread->mut.unlock();
	};
	void assign_on_kill(Thread* thread, Task task)
	{
		thread->mut.lock();
		thread->onKill.emplace_back(task);
		thread->mut.unlock();
	};


	Task _load_shader(Renderable& renderable, std::string file_name) noexcept
	{
		return [&renderable, file_name]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				if (!window_to_shader[window])
					window_to_shader[window] = new ShaderManager();
				renderable.shader = window_to_shader[window]->create(file_name);
			};
		};
	};
	Task _load_shader(Renderable& renderable, std::vector<std::string> file_names) noexcept
	{
		return [&renderable, file_names]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				if (!window_to_shader[window])
					window_to_shader[window] = new ShaderManager();
				renderable.shader = window_to_shader[window]->create(file_names);
			};
		};
	};
	Task _load_textures(Renderable& renderable, std::vector<std::pair<std::string, unsigned int>> texture_files) noexcept
	{
		return [&renderable, texture_files]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				if (!window_to_texture[window])
					window_to_texture[window] = new TextureManager();
				renderable.textures = window_to_texture[window]->create(texture_files);
			};
		};
	};
	Task _render_func(Buffer<Renderable>& buffer) noexcept
	{
		return buffer._view([](const Renderable& r)
		{
			glUseProgram(r.shader);

			for (auto& texture : r.textures)
				glBindTexture(GL_TEXTURE_2D, texture); // TODO: Make generic (not just GL_TEXTURE_2D)

			for (auto& mesh : r.meshes)
			{
				glBufferData(
					GL_ARRAY_BUFFER,
					mesh.vtxs.size() * sizeof(Vtx),
					mesh.vtxs.data(),
					GL_DYNAMIC_DRAW);

				glBufferData(
					GL_ELEMENT_ARRAY_BUFFER,
					mesh.inds.size() * sizeof(Ind),
					mesh.inds.data(),
					GL_STATIC_DRAW);

				glDrawElements(r.draw_mode, (GLsizei)mesh.inds.size(), GL_UNSIGNED_INT, nullptr);
			};

			for (auto& texture : r.textures)
				glBindTexture(GL_TEXTURE_2D, 0);
		});
	};


	void run()
	{
		run(threads);
	};


	void run(std::unordered_set<Thread*> threads)
	{
		for (auto& thread : threads)
			thread->thread = std::thread([thread]()
			{
				thread->mut.lock();
				thread->KILL = false;
				for (auto& task : thread->onInvk)
					task();
				thread->mut.unlock();

				while (!thread->KILL)
				{
					if (!thread->to_add.empty())
					{
						thread->mut.lock();
						thread->tasks.reserve(thread->to_add.size());
						for (auto& task : thread->to_add)
							thread->tasks.emplace_back(task);
						for (auto& task : thread->singletons)
							task();
						thread->singletons.clear();
						thread->to_add.clear();
						thread->mut.unlock();
					};

					for (auto& task : thread->tasks)
						task();
				};

				thread->mut.lock();
				for (auto& task : thread->onKill)
					task();
				thread->mut.unlock();
			});

		for (auto& thread : threads)
		{
			if (thread->thread.joinable())
				thread->thread.join();
		};

		glfwTerminate();
	};


	struct Sync
	{
		static inline void sync(std::unordered_set<Thread*> threads,
								std::shared_ptr<Barrier> barrier,
								Task on_cycle = []() noexcept { })
		{
			for (auto& thread : threads)
			{
				assign(thread, barrier->arrive_and_wait);
				assign_on_kill(thread, barrier->arrive_and_drop);
			};
		};

	private:
		Sync() { };
	};
	void sync(std::unordered_set<Thread*>	threads,
			  std::shared_ptr<Barrier>		barrier,
			  Task							on_cycle)
	{
		Sync::sync(threads, barrier, on_cycle);
	};


	void close()
	{
		close(threads);
	};


	void close(std::unordered_set<Thread*> threads)
	{
		for (auto& thread : threads)
			thread->KILL = true;
	};


	Thread* create_thread()
	{
		Thread* thread = new Thread();
		threads.emplace(thread);

		return thread;
	};


	void destroy(Thread* thread)
	{
		thread->KILL = true;

		if (thread->thread.joinable())
			thread->thread.join();

		threads.erase(thread);
		delete thread;
		thread = nullptr;
	};
};
