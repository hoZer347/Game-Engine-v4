#include "Input.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Utils.h"

namespace loom
{
	static inline std::shared_ptr<Inputs> INPUT = std::make_shared<Inputs>();
	Inputs::Inputs(std::shared_ptr<Inputs> _prev)
	: _prev(_prev)
	{ };
	void Inputs::next()
	{
		INPUT = std::make_shared<Inputs>(INPUT);
	};
	void Inputs::prev()
	{
		INPUT = INPUT->_prev;
	};
	void Inputs::MouseButtonPress(Task&& task, Input&& input)
	{
		INPUT->inputs[input] = task;
	};
	void Inputs::KeyPress(Task&& task, Input&& input)
	{
		INPUT->inputs[input] = task;
	};
	void Inputs::MouseButtonHold(Task&& task, Input&& input)
	{
		INPUT->mbns.emplace_back(input, task);
	};
	void Inputs::KeyHold(Task&& task, Input&& input)
	{
		INPUT->keys.emplace_back(input, task);
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
	void Inputs::clear()
	{
		INPUT->keys.clear();
		INPUT->mbns.clear();
		INPUT->inputs.clear();
	};
	void Inputs::load()
	{
		if (GLFWwindow* window = glfwGetCurrentContext())
		{
			glfwSetMouseButtonCallback(window, [](GLFWwindow*, GLint button, GLint action, GLint mods)
			{
				Input i{ { (uint16_t)0, (uint16_t)button, (uint16_t)action, (uint16_t)mods } };
				if (INPUT->inputs[i.input])
					INPUT->inputs[i.input]();
			});
			glfwSetKeyCallback(window, [](GLFWwindow*, GLint key, GLint scancode, GLint action, GLint mods)
			{
				Input i{ { (uint16_t)key, (uint16_t)action, (uint16_t)scancode, (uint16_t)mods } };
				if (INPUT->inputs[i.input])
					INPUT->inputs[i.input]();
			});
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
			for (auto& i : INPUT->keys)
				if (glfwGetKey(window, i.first.data.key) == i.first.data.action)
					i.second();
			//



			// Process all mouse buttons being pressed
			for (auto& i : INPUT->mbns)
				if (glfwGetMouseButton(window, i.first.data.button) == i.first.data.action)
					i.second();
			//


			// Recording the previous mouse / scroll position
			pmx = mx;
			pmy = my;
			psx = sx;
			psy = sy;
			//
		};
	};
};
