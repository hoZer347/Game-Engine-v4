#pragma once

#include "Data.h"

#include <mutex>
#include <queue>
#include <vector>
#include <thread>
#include <memory>
#include <functional>

#include <iostream>

struct GLFWwindow;

#define print(...) std::cout << __VA_ARGS__ << std::endl;

namespace eng
{
	typedef std::shared_ptr<std::function<bool()>> Task;
	#define create_task(task) std::make_shared<std::function<bool()>>(task)

	struct TextureManager;
	struct ShaderManager;

	struct Camera;

	struct Thread
	{
		virtual ~Thread();

		void assign(Task task);
		void assign_on_kill(Task task);

		void assign(std::function<bool()> task);
		void assign_on_kill(std::function<bool()> task);

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
		static void sync(std::vector<std::shared_ptr<Thread>> threads)
		{
			for (auto& thread : threads)
				thread->assign([]()
					{
						if (count >= 0)
						{
							count++;
							while (count < SIZE || PAUSE);
							count--;
						};

						return true;
					});
		};

		static void desync()
		{
			count = -1;
		};

		static void pause(bool wait = false)
		{
			PAUSE = true;
			while (wait && !(count < SIZE));
		};

		static void play()
		{
			PAUSE = false;
		};

	private:
		static inline std::atomic<int>	count = 0;
		static inline std::atomic<bool>	PAUSE = false;
		Sync() { };
	};

	void start();
	void close();
};
