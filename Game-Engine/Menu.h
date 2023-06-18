#pragma once

#include "Data.h"

#include <list>

namespace loom
{
	struct Control;
	struct Rect;

	struct Option
	{
		virtual ~Option() { };

		Task on_select	= []() {};
		Task on_hover	= []() {};
		Task on_unhover = []() {};
		std::function<bool()> hover_detection = []() { return true; };
	};


	struct Menu final
	{
		Menu(Control& control);
		static std::shared_ptr<Menu> make(Control& control);

		void next();
		void prev();
		void leave();

		void AddOption(std::shared_ptr<Option> option);

		template <typename... Options>
		void AddOptions(std::shared_ptr<Option> option, Options... options);

	private:
		static void AddOptions() { };
		
		Control& control;
		std::shared_ptr<Option> hovered = nullptr;
		std::list<std::vector<std::shared_ptr<Option>>> options;
		std::recursive_mutex mut;
	};


	struct RectOption final :
		virtual public Option
	{
		RectOption(float&& x, float&& y, float&& w, float&& h);
	};
};
