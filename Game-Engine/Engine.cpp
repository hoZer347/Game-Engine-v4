#include "Engine.h"

#include "Mem_Leak.h"

#include "GLFW/glfw3.h"

#include <iostream>
#include <vector>

namespace eng
{
	void init()
	{

	};

	void close()
	{
		glfwTerminate();
	};

	void Thread::init()
	{
		t = std::thread([this]() {
			while (!KILL)
			{
				if (!tasks.empty())
				{
					tasks.front()->exec();
					tasks.pop();
				}

				while (PAUSE) if (KILL) break;
			};

			while (!tasks.empty())
				tasks.pop();
			});
	};

	void Thread::join()
	{
		t.join();
	};

	void Thread::assign(Task* t)
	{
		std::lock_guard<std::mutex> guard (mut);

		tasks.push(t);
	};
};
