#include "Input.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Utils.h"

namespace loom
{
	// TODO: scancode / action ordering

	static inline std::shared_ptr<Input> INPUT = std::make_shared<Input>();
	Input::Input(std::shared_ptr<Input> _prev)
	: _prev(_prev)
	{ };
	void Input::next()
	{
		INPUT = std::make_shared<Input>(INPUT);
	};
	void Input::prev()
	{
		INPUT = INPUT->_prev;
	};
	void Input::MouseButtonPress(Task task, uint16_t button, uint16_t action, uint16_t mods)
	{
		input i{ { 0, button, action, mods } };
		INPUT->inputs[i.input] = task;
	};
	void Input::KeyPress(Task task, uint16_t key, uint16_t action, uint16_t scancode, uint16_t mods)
	{
		input i{ { key, action, scancode, mods } };
		INPUT->inputs[i.input] = task;
	};
	void Input::MouseButtonHold(Task task, uint16_t button, uint16_t action, uint16_t mods)
	{
		input i{ { 0, button, action, mods } };
		INPUT->mbns.emplace_back(i, task);
	};
	void Input::KeyHold(Task task, uint16_t key, uint16_t action, uint16_t scancode, uint16_t mods)
	{
		input i{ { key, action, scancode, mods } };
		INPUT->keys.emplace_back(i, task);
	};
	void Input::GetMousePos(double& mx, double& my)
	{
		mx = Input::mx;
		my = Input::my;
	};
	void Input::GetRelativeMousePos(double& mx, double& my)
	{
		mx = Input::mx - Input::pmx;
		my = Input::my - Input::pmy;
	};
	void Input::GetScrollPos(double& sx, double& sy)
	{
		sx = Input::sx;
		sy = Input::sy;
	};
	void Input::GetRelativeScrollPos(double& sx, double& sy)
	{
		sx = Input::sx - Input::psx;
		sy = Input::sy - Input::psy;
	};
	void Input::AddTask(Task task)
	{
		INPUT->tasks.push_back(task);
	};
	void Input::load()
	{
		if (GLFWwindow* window = glfwGetCurrentContext())
		{
			glfwSetMouseButtonCallback(window, [](GLFWwindow*, GLint button, GLint action, GLint mods)
			{
				input i{ { (uint16_t)0, (uint16_t)button, (uint16_t)action, (uint16_t)mods } };
				if (INPUT->inputs[i.input])
					INPUT->inputs[i.input]();
			});
			glfwSetKeyCallback(window, [](GLFWwindow*, GLint key, GLint scancode, GLint action, GLint mods)
			{
				input i{ { (uint16_t)key, (uint16_t)action, (uint16_t)scancode, (uint16_t)mods } };
				if (INPUT->inputs[i.input])
					INPUT->inputs[i.input]();
			});
			glfwSetScrollCallback(window, [](GLFWwindow* window, double sx, double sy)
			{
				Input::sx += sx;
				Input::sy += sy;
			});
			glfwSetCursorPosCallback(window, [](GLFWwindow*, double mx, double my)
			{
				Input::mx = mx;
				Input::my = my;
			});
		};
	};
	void Input::update()
	{
		static Utils::Timer timer;

		if (timer.GetDiff_mls() < 1000 / INPUT_TICKRATE)
			return;
		timer.push(std::chrono::milliseconds(1000 / INPUT_TICKRATE));
		
		if (GLFWwindow* window = glfwGetCurrentContext())
		{
			glfwPollEvents();
			


			// Process all miscellanious tasks
			for (auto& task : INPUT->tasks)
				task();
			//
			


			// Process all keys currently being pressed
			for (auto& i : INPUT->keys)
				if (glfwGetKey(window, i.first.data[0]) == i.first.data[1])
					i.second();
			//



			// Process all mouse buttons being pressed
			for (auto& i : INPUT->mbns)
				if (glfwGetMouseButton(window, i.first.data[1]) == i.first.data[2])
					i.second();
			//



			// Recording the previous mouse / scroll position
			pmx = mx;
			pmy = my;
			psx = sx;
			psy = sy;
			//



			// TODO: add scancode bullshit
			// TODO: add controller support
		};
	};
};
