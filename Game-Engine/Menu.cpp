#include "Menu.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Input.h"

#include <mutex>

namespace loom
{
	std::shared_ptr<Menu> MENU = nullptr;
	static inline std::recursive_mutex mut;


	void Menu::Init()
	{

	};
	void Menu::Exit()
	{

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
