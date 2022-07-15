#include "Engine.h"

#include "Mem_Leak.h"

#include "Shader.hpp"
#include "Texture.hpp"
#include "Window.h"

#include <iostream>
#include <vector>

namespace eng
{
	void init()
	{
		stbi_set_flip_vertically_on_load(true);
	};

	void close()
	{
		for (auto& t : Thread::all_threads)
			if (t->joinable()) {
				t->KILL = true;
				t->join();
			}

		glfwTerminate();
	};

	void Thread::init()
	{
		t = std::thread([this]() {
			onInit();

			while (!KILL)
			{
				if (!tasks.empty())
				{
					tasks.front()->exec();
					tasks.pop();
				}

				for (auto& t : kernel)
					t->exec();

				exec();

				while (PAUSE) if (KILL) break;
			};

			onKill();
			});
	};
	void Thread::join()
	{
		t.join();
	};
	void Thread::push(Task* t)
	{
		std::lock_guard<std::mutex> guard (mut1);

		tasks.push(t);
	};
	void Thread::assign(Task* t)
	{
		std::lock_guard<std::mutex> guard(mut2);

		kernel.push_back(t);
	};
	void Thread::clear()
	{
		kernel.clear();
	};

	void debug()
	{

	};
};
