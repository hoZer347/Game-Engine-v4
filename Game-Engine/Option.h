#pragma once

#include "Data.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

namespace loom
{
	// Option to be used by Menus
	// on_hover: Executes once whenever hover_check passes
	// on_unhover: Executes once whenever another object's hover_check passes, and this ones doesn't
	// on_select: Exectues once whenever this object is hovered and selected
	// hover_check: Determines whether or not this object is hovered
	struct Option final
	{
		Task on_hover    = [](){};
		Task on_unhover  = [](){};
		Task on_select   = [](){};
		std::function<bool()> hover_check = []() { return false; };
	};



	template <uint32 input>
	ptr<Option> CheckForPressed(Task on_select, Task on_hover = []() { }, Task on_unhover = []() { })
	{
		return ptr<Option> {
			1,
			on_hover,
			on_unhover,
			on_select,
			[]()
			{
				return (bool)Control::inputs[input];
			}
		};
	};
};
