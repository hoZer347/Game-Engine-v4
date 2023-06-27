#pragma once

#include "Control.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

namespace loom
{
	typedef unsigned int uint32;

	struct Option
	{
		void(*on_hover)() = []() {};
		void(*on_unhover)() = []() {};
		void(*on_select)() = []() {};
		bool(*hover_check)() = []() { return false; };
	};

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
