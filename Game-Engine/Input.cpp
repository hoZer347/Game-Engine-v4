#include "Input.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Utils.h"

namespace loom
{
	static inline thread_local std::shared_ptr<Inputs> INPUT = nullptr;

	Inputs::Inputs(std::shared_ptr<Inputs> _prev)
	: _prev(_prev)
	{ };
	void Inputs::next()
	{
		INPUT = std::make_shared<Inputs>(INPUT);
	};
	void Inputs::prev()
	{
		for (auto& task : INPUT->on_prev)
			task();

		INPUT = INPUT->_prev;
	};
	void Inputs::AddInput(Task&& task, Input&& input)
	{
		INPUT->inputs.emplace_back(input, task);
	};
	void Inputs::RmvInput(Input&& input)
	{
		INPUT->inputs.erase(
			std::remove_if(
				INPUT->inputs.begin(),
				INPUT->inputs.end(),
				[&input](auto& p)
				{
					return p.first == input;
				}));
	};
	void Inputs::GetMousePos(double& mx, double& my)
	{
		mx = Inputs::mx;
		my = Inputs::my;
	};
	void Inputs::GetRelativeMousePos(double& mx, double& my)
	{
		mx = Inputs::mx - Inputs::pmx;
		my = Inputs::my - Inputs::pmy;
	};
	void Inputs::GetScrollPos(double& sx, double& sy)
	{
		sx = Inputs::sx;
		sy = Inputs::sy;
	};
	void Inputs::GetRelativeScrollPos(double& sx, double& sy)
	{
		sx = Inputs::sx - Inputs::psx;
		sy = Inputs::sy - Inputs::psy;
	};
	void Inputs::AddOnPrev(Task task)
	{
		INPUT->on_prev.emplace_back(task);
	};
	void Inputs::clear()
	{
		INPUT->inputs.clear();
	};
	void Inputs::load()
	{
		if (GLFWwindow* window = glfwGetCurrentContext())
		{
			glfwSetScrollCallback(window, [](GLFWwindow* window, double sx, double sy)
			{
				Inputs::sx += sx;
				Inputs::sy += sy;
			});
			glfwSetCursorPosCallback(window, [](GLFWwindow*, double mx, double my)
			{
				Inputs::mx = mx;
				Inputs::my = my;
			});
			glfwSetWindowSizeCallback(window, [](GLFWwindow*, int w, int h)
			{
				glViewport(0, 0, w, h);
			});
		};
	};
	void Inputs::update()
	{
		static Utils::Timer timer;

		if (timer.GetDiff_mls() < 1000 / INPUT_TICKRATE)
			return;
		timer.push(std::chrono::milliseconds(1000 / INPUT_TICKRATE));
		
		if (GLFWwindow* window = glfwGetCurrentContext())
		{
			glfwPollEvents();
			

			// Process all keys currently being pressed
			for (auto& i : INPUT->inputs)
				if (glfwGetKey(window, i.first.data.key) == i.first.data.action)
					i.second();


			// Process all mouse buttons being pressed
			for (auto& i : INPUT->inputs)
				if (glfwGetMouseButton(window, i.first.data.button) == i.first.data.action)
					i.second();


			// Recording the previous mouse / scroll position
			pmx = mx;
			pmy = my;
			psx = sx;
			psy = sy;
			//
		};
	};
};
