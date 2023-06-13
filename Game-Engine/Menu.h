#pragma once

#include "Data.h"

namespace loom
{
	struct Option final
	{
		Task on_select;
		std::function<bool()> hover_detection;
		Task on_hover;
		Task on_unhover;
	};

	struct Menu final
	{
		static void next();
		static void prev();

		static void AddOptions(Option* options...);

	private:
		Menu() { };
		static void Init();
		static void Exit();
		std::shared_ptr<Menu> _prev = nullptr;
	};
};
