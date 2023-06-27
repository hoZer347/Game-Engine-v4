#pragma once

#include "Data.h"
#include "Control.h"

#include <functional>

namespace loom
{
	struct Option
	{
		Task on_hover	= [](){};
		Task on_unhover = [](){};
		Task on_select	= [](){};
		std::function<bool()> hover_check = []() { return false; };
	};

	struct Menu final
	{
		static _NODISCARD ptr<Control>&& next(std::vector<Option>&& options);
		static void prev();
		static void leave();

		static inline std::vector<Option> options;
	};


	inline ptr<Control>&& Menu::next(std::vector<Option>&& new_options)
	{
		return Control::next([new_options]()
		{
			options = new_options;

			Control::AddOnReenter([new_options]()
			{
				options = new_options;
			});
			Control::AddOnLeave([]()
			{
				options.clear();
			});
		});
	};
	inline void Menu::prev()
	{

	};
	inline void Menu::leave()
	{
		
	};
};
