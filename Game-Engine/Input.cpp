#include "Engine.h"
#include "Thread.h"

#include "GLFW/glfw3.h"

#include <unordered_map>
#include <unordered_set>
#include <stack>

#include <thread>
#include <mutex>

#include <iostream>

namespace eng
{
	union i_conv
	{
		_int64 index;
		_int16 input[4];
	};

	Thread* thread;
	static inline std::stack<std::unordered_map<_int64, Task>> inputs;

	namespace input
	{
		void open(Window* source)
		{
			thread = (Thread*)source;

			((Thread*)source)->push([]()
				{
					GLFWwindow* window = glfwGetCurrentContext();

					glfwSetMouseButtonCallback(window, [](GLFWwindow*, int binding, int action, int mods)
						{
							invoke(binding, action, mods);
						});

					glfwSetKeyCallback(window, [](GLFWwindow*, int binding, int, int action, int mods)
						{
							invoke(binding, action, mods);
						});

					return false;
				});

			// TODO: add on_update function here
 		};

		void bind(Task task, int binding, int action, int mods)
		{
			thread->push([task, binding, action, mods]()
				{
					i_conv i = { .input = { (short)binding, (short)action, (short)mods, 0 } };

					if (!inputs.empty())
						inputs.top()[i.index] = task;

					return false;
				});
		};

		void bind(Task on_press, Task on_release, int binding, int mods)
		{
			bind(on_press, binding, GLFW_PRESS, mods);
			bind(on_release, binding, GLFW_RELEASE, mods);
		};

		void bind(Task on_hold, bool& flag, int binding, int mods)
		{
			input::bind([&flag, on_hold]()
				{
					flag = true;

					thread->push(on_hold);

					return false;
				}, [&flag]()
				{
					flag = false;

					return false;
				}, binding, mods);
		};

		void invoke(int binding, int action, int mods)
		{
			thread->push([binding, action, mods]()
			{
				if (!inputs.empty())
				{
					i_conv i = { .input = { (short)binding, (short)action, (short)mods, 0 } };

					if (inputs.top().count(i.index))
						inputs.top()[i.index]();
				};

				return false;
			});
		};

		void set_interval(long long interval)
		{
			// TODO:
		};

		void next()
		{
			thread->push([]()
			{
				inputs.push({});

				return false;
			});
		};

		void prev()
		{
			thread->push([]()
			{
				if (!inputs.empty())
					inputs.pop();

				return false;
			});
		};
	};
};
