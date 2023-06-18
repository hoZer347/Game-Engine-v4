#include "Menu.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Control.h"

namespace loom
{
	Menu::Menu(Control& control) :
		control(control)
	{
		next();
	};
	std::shared_ptr<Menu> Menu::make(Control& control)
	{
		return std::shared_ptr<Menu>(new Menu(control));
	};
	void Menu::next()
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		options.emplace_back();
		if (options.size() == 1)
		{
			control.next([this]()
			{
				this->control.AddTask([this]()
				{
					std::scoped_lock<std::recursive_mutex> lock{mut};

					if (options.empty())
					{
						this->control.prev();
						return;
					};

					for (auto& option : options.back())
						if (option->hover_detection())
						{
							// Skipping if hovered option is still hovered
							if (hovered == option)
								return;

							// Unhovering last option
							if (hovered)
								hovered->on_unhover();

							// Hovering current option
							if (hovered != option)
								option->on_hover();
							
							hovered = option;

							return;
						};

					if (hovered)
						hovered->on_unhover();

					hovered = nullptr;
				});
				this->control.AddOutput([this](float& f)
				{
					if (f && hovered)
						hovered->on_select();

				}, GLFW_MOUSE_BUTTON_LEFT);
				this->control.AddOnPrev([this]() { options.clear(); });
			});
		};
	};
	void Menu::prev()
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		options.pop_back();
	};
	void Menu::leave()
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		control.prev();
	};
	void Menu::AddOption(std::shared_ptr<Option> option)
	{
		std::scoped_lock<std::recursive_mutex> lock{mut};

		options.back().push_back(option);
	};
	template <typename... Options>
	void Menu::AddOptions(std::shared_ptr<Option> option, Options... options)
	{
		AddOption(option);
		AddOptions(options...);
	};


	RectOption::RectOption(float&& x, float&& y, float&& w, float&& h)
	{
		hover_detection = [x, y, w, h]()
		{
			return
				Control::mx > x &&
				Control::my > y &&
				Control::mx < x + w &&
				Control::my < y + h;
		};
	};
};
