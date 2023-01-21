#include "Input.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

namespace loom
{
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
	void Input::KeyPress(Task task, uint16_t key, uint16_t scancode, uint16_t action, uint16_t mods)
	{
		input i{ { key, scancode, action, mods } };
		INPUT->inputs[i.input] = task;
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
				input i{ { (uint16_t)key, (uint16_t)scancode, (uint16_t)action, (uint16_t)mods } };
				if (INPUT->inputs[i.input])
					INPUT->inputs[i.input]();
			});
		};
	};
	void Input::update()
	{
		// TODO: Add Scans Here
	};
};
