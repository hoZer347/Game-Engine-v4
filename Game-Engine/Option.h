#pragma once

#include "Control.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

namespace loom
{
	typedef unsigned int uint32;

	// Option to be used by Menus
	// on_hover: Executes once whenever hover_check passes
	// on_unhover: Executes once whenever another object's hover_check passes, and this ones doesn't
	// on_select: Exectues once whenever this object is hovered and selected
	// hover_check: Determines whether or not this object is hovered
	struct Option
	{
		void(*on_hover)() = []() {};
		void(*on_unhover)() = []() {};
		void(*on_select)() = []() {};
		bool(*hover_check)() = []() { return false; };
	};

	// Option that is hovered whenever the given input is pressed
	template <uint32 input>
	struct CheckForPressed final : public Option
	{
		CheckForPressed() : Option()
		{
			hover_check = []()
			{
				return (bool)inputs[input];
			};
		};
	};
};
