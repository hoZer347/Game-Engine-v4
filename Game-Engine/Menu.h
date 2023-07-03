#pragma once

#include "Data.h"
#include "Control.h"
#include "Option.h"

#include <functional>

namespace loom
{
	struct Menu final
	{
		// Opens a new menu, creating a new control layer
		static void next(auto&& option, auto&&... options);
		
		// Goes to previous control layer of the menu
		static void prev();

		// Relinquishes any menu control
		static void leave();
		
		static inline std::shared_ptr<Control> control;

	private:
		static void next();
		
		static inline std::vector<ptr<Option>> options;
		static inline ptr<Option> hovered;
	};


	inline void Menu::next()
	{
		control = Control::next();
		
		control->AddTask([]()
		{
			if (Control::inputs[GLFW_MOUSE_BUTTON_2])
			{
				Menu::leave();
				return;
			};

			if (Control::inputs[GLFW_MOUSE_BUTTON_1] && hovered)
			{
				hovered->on_select();
				Menu::leave();
				return;
			};

			for (auto& option : options)
				if (option->hover_check())
				{
					if (option != hovered)
					{
						if (hovered)
							hovered->on_unhover();
						option->on_hover();
						hovered = option;
					};

					return;
				};
		});
	};
	inline void Menu::next(auto&& option, auto&&... options)
	{
		Menu::options.push_back(static_cast<ptr<Option>>(option));
		Menu::next(options...);
	};
	inline void Menu::prev()
	{
		assert((Menu::control, "Tried to .prev() an empty control layer"));
		Menu::control->prev();
	};
	inline void Menu::leave()
	{
		assert((Menu::control, "Menu is already out of control"));
		Menu::control.reset();
	};
};
