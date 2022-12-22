#include "Menu.h"

#include "Engine.h"

// OpenGL Includes
#include "GLFW/glfw3.h"
#include "glm/gtx/transform.hpp"
using namespace glm;
//

namespace loom
{
	Menu::Menu(Thread* window, Thread* thread)
	{
		assign(window, _handle());

		if (thread != nullptr)
			assign(thread, _update());
		else
			assign(window, _update());
	};
	void Menu::add(std::vector<MenuItem*> items)
	{
		this->items.reserve(items.size());
		this->items.insert(this->items.end(), items.begin(), items.end());
	};
	Task Menu::_update()
	{
		return [this]()
		{
			for (auto& item : items)
				item->update();
		};
	};
	Task Menu::_handle()
	{
		return [this]()
		{
			if (GLFWwindow* window = glfwGetCurrentContext())
			{
				double _xpos, _ypos;
				glfwGetCursorPos(window, &_xpos, &_ypos);
				xpos = _xpos;
				ypos = _ypos;

				for (auto& item : items)
					item->handle();
			};
		};
	};
};
