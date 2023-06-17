#include "Control.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Utils.h"

#include <fstream>

namespace loom
{
	struct ControlManager :
		virtual protected Loadable,
		virtual protected Renderable
	{
		void load() override
		{
			Control::inputs.fill(0);

			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				glfwSetScrollCallback(window, [](GLFWwindow* window, double _sx, double _sy)
				{
					Control::sx += _sx;
					Control::sy += _sy;
				});
				glfwSetWindowSizeCallback(window, [](GLFWwindow*, int w, int h)
				{
					glViewport(0, 0, w, h);
				});
				glfwSetKeyCallback(window, [](GLFWwindow*, int button, int, int action, int)
				{
					Control::inputs[button] = (float)action;
				});
				glfwSetMouseButtonCallback(window, [](GLFWwindow*, int button, int action, int)
				{
					Control::inputs[button] = (float)action;
				});
			};
		};
		void render() override
		{
			static Utils::Timer timer;

			if (timer.GetDiff_mls() < 1000.f / CONTROL_TICKRATE)
				return;
			timer.push(std::chrono::milliseconds(1000 / CONTROL_TICKRATE));

			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				Control::pmx = Control::mx;
				Control::pmy = Control::my;
				glfwGetCursorPos(window, &Control::mx, &Control::my);
				glfwPollEvents();
			};
		};
	};
	static inline ControlManager manager;


	void Control::next(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		if (data)
			for (auto& task : data->on_next)
				task();

		std::shared_ptr<Data> data = std::shared_ptr<Data>(new Data());
		data->_prev = this->data;
		this->data = data;

		task();
	};
	void Control::prev(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		for (auto& task : data->on_prev)
			task();

		data = data->_prev;

		for (auto& task : data->on_reenter)
			task();

		task();
	};
	void Control::reset(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		data = std::shared_ptr<Data>(new Data());

		task();
	};
	void Control::clear()
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		data->tasks.clear();
		data->on_next.clear();
		data->on_prev.clear();
		data->io.clear();
	};
	void Control::AddTask(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		data->tasks.emplace_back(task);
	};
	void Control::AddOnNext(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		data->on_next.emplace_back(task);
	};
	void Control::AddOnPrev(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		data->on_prev.emplace_back(task);
	};
	void Control::AddOnReenter(Task&& task)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		data->on_reenter.emplace_back(task);
	};
	void Control::AddOutput(Output&& output, Input&& input)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};
		data->io.emplace_back(input, output);
	};
	void Control::update()
	{
		static Utils::Timer timer;

		if (timer.GetDiff_mls() < 1000.f / CONTROL_TICKRATE)
			return;
		timer.push(std::chrono::milliseconds(1000 / CONTROL_TICKRATE));

		for (auto& task : data->tasks)
			task();
		for (auto& i : data->io)
			i.second(inputs[i.first]);
	};
};
