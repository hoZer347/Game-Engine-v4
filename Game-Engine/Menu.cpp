#include "Menu.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Input.h"

namespace loom
{
	static inline std::shared_ptr<Menu> MENU = nullptr;

	void Menu::Init()
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};


	};
	void Menu::Exit()
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};


	};
	void Menu::next()
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		if (!MENU)
		{
			MENU = std::shared_ptr<Menu>(new Menu());
			Menu::Init();
		}
		else
		{
			std::shared_ptr<Menu> menu = std::shared_ptr<Menu>(new Menu());
			menu->_prev = MENU;
			MENU = menu;
		};
	};
	void Menu::prev()
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		if (!(MENU = MENU->_prev))
			Menu::Exit();
	};
	void Menu::AddOptions(Option* options ...)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};


	};
};
