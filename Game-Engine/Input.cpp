#include "Engine.h"
#include "Thread.h"

#include "GLFW/glfw3.h"

#include <unordered_map>
#include <unordered_set>
#include <stack>

#include <thread>
#include <mutex>

#include <iostream>

static inline std::mutex mut;

namespace eng
{
	union i_conv
	{
		_int64 index;
		_int16 input[4];
	};

	inline _int64 conv(_int16 binding, _int16 action, _int16 mods)
	{
		return i_conv({ .input = { binding, action, mods, 0 } }).index;
	};

	static inline std::stack<std::unordered_map<_int64, Task>> task_stack;

	namespace input
	{
		void open(Window* source)
		{
			static_cast<Thread*>((void*)source)->push([]()
				{
					GLFWwindow* window = glfwGetCurrentContext();

					glfwSetMouseButtonCallback(window, [](GLFWwindow*, int binding, int action, int mods)
						{
							if (task_stack.empty())
								return;

							_int64 i = conv(binding, action, mods);

							if (task_stack.top().count(i))
								task_stack.top()[i];
						});

					glfwSetKeyCallback(window, [](GLFWwindow*, int binding, int, int action, int mods)
						{
							
						});

					return false;
				});
		};

		void close()
		{
			
		};

		void bind(Task task, int binding, int action, int mods)
		{
			
		};

		void set_interval(long long interval)
		{
			// TODO:
		};

		void next()
		{
			mut.lock();
			task_stack.push({});
			mut.unlock();
		};

		void prev()
		{
			mut.lock();
			task_stack.pop();
			mut.unlock();
		};
	};
};
