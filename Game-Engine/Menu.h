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
		static void next(std::vector<Option>&& options, bool(*select_check)());
		
		// Goes to previous control layer of the menu
		static void prev();

		// Relinquishes any menu control
		static void leave();
		
	private:
		static inline std::shared_ptr<Control> control;
		static inline bool(*select_check)() = []() { return false; };
		static inline std::vector<Option> options;
		static inline Option* hovered = nullptr;
	};


	inline void Menu::next(std::vector<Option>&& new_options, bool(*select_check)())
	{
		Menu::control = Control::next([new_options, select_check]()
		{
			Menu::options = new_options;
			Menu::select_check = select_check;

			Control::AddOnReenter([new_options, select_check]()
			{
				Menu::options = new_options;
				Menu::select_check = select_check;
			});
			Control::AddOnLeave([]()
			{
				hovered = nullptr;
				options.clear();
			});
			Control::AddTask([]()
			{
				for (auto& option : options)
					if (option.hover_check())
					{
						if (&option != Menu::hovered)
						{
							if (Menu::hovered)
								hovered->on_unhover();
							option.on_hover();
							hovered = &option;
						};

						if (Menu::select_check())
							option.on_select();

						return;
					};

				hovered = nullptr;
			});
		});
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
