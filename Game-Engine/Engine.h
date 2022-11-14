#pragma once

#include "Data.h"

#include <mutex>
#include <queue>
#include <vector>
#include <thread>
#include <memory>
#include <barrier>
#include <functional>

#include <iostream>

struct GLFWwindow;

#define print(...) std::cout << __VA_ARGS__ << std::endl;

namespace eng
{
	typedef std::shared_ptr<std::function<void()>> Task;
	#define create_task(task) std::make_shared<std::function<void()>>(task)

	struct TextureManager;
	struct ShaderManager;

	struct Camera;

	struct Thread
	{
		virtual ~Thread();

		void push(Task task);
		void assign(Task task);
		void assign_on_kill(Task task);

		void push(std::function<void()> task);
		void assign(std::function<void()> task);
		void assign_on_kill(std::function<void()> task);

		void modify(std::function<void(std::vector<Task>&)> vec_func);
		
		void clean();

		bool get_kill() { return KILL; };

	protected:
		friend struct Data<Thread>;
		Thread();

		bool				KILL = false;
		std::thread			thread;

	private:
		std::mutex			mut;

		std::queue<Task>	singletons;
		std::vector<Task>	on_kill;
		std::vector<Task>	tasks;
	};

	struct Window : public Thread
	{
		virtual ~Window();

		void load_shader(std::vector<std::string> file_names, unsigned int& shader);
		void load_textures(std::vector<std::pair<std::string, unsigned int>> file_names, std::vector<unsigned int>& textures);

	protected:
		friend struct Data<Window>;
		Window();

	private:
		std::shared_ptr<TextureManager>	t_mgr;
		std::shared_ptr<ShaderManager>	s_mgr;
		std::shared_ptr<Camera>			camera;

		unsigned int	_vtxs=0, _inds=0;
		GLFWwindow*		glfw_window	= nullptr;
	};

	template <size_t ID, size_t SIZE>
	struct Sync
	{
		
		// Syncs together a number of threads, return the task they use to do so
		// delete the returned task to stop the syncing
		static inline Task create(
			std::vector<std::shared_ptr<Thread>> threads,
			bool pauseable = false)
		{
			Task ret;

			threads[0]->assign_on_kill([]()
				{
					Sync<ID, SIZE>::task.reset();
				});

			if (pauseable)
				ret = std::make_shared<std::function<void()>>([]()
					{
						Sync<ID, SIZE>::bar.arrive_and_wait();
						while (Sync<ID, SIZE>::PAUSE);
					});
			else
				ret = std::make_shared<std::function<void()>>([]()
					{
						Sync<ID, SIZE>::bar.arrive_and_wait();
					});
			
			for (auto& thread : threads)
				thread->assign(ret);

			return ret;
		};

		// Set the task to run at the start of every cycle
		static inline void set_task(Task t)
		{
			mut.lock();
			task = t;
			mut.unlock();
		};

		// Resume the threads
		static inline void play()
		{
			PAUSE = false;
		}

		// Pause the threads
		static inline void pause()
		{
			PAUSE = true;
		}

	private:
		static inline std::atomic<bool>	PAUSE = true;
		static inline Task				task = create_task([]() {});
		static inline std::mutex		mut;
		static inline std::barrier		bar = std::barrier(SIZE, []() noexcept
			{
				Sync<ID, SIZE>::mut.lock();
				(*Sync<ID, SIZE>::task)();
				Sync<ID, SIZE>::mut.unlock();
			});
		Sync() { };
	};

	void start();
	void close();
};
