#pragma once

#include "Data.h"
#include "Task.h"

#include <mutex>
#include <queue>
#include <vector>
#include <thread>
#include <memory>
#include <functional>

#include <iostream>

struct GLFWwindow;

namespace eng
{
	struct TextureManager;
	struct ShaderManager;

	struct Camera;

	struct Thread
	{
		virtual ~Thread();

		void assign(Task task);

		void kill() { KILL = true; }
		
		void clean();

		inline bool get_kill() { return KILL; };

		inline size_t size(bool ret_on_kill = false)
		{
			size_t ret;

			mut.lock();
			ret = tasks.size();
			mut.unlock();

			return ret;
		};

	protected:
		friend struct Data<Thread>;
		Thread();

		std::atomic<bool>					KILL = false;
		std::thread							thread;

	private:
		std::atomic<size_t>					num_procs;
		std::mutex							mut;
		std::vector<std::shared_ptr<Task>>	tasks;
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
