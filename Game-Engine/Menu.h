#pragma once

#include "Data.h"
#include "Control.h"
#include "Option.h"

#include <functional>

namespace loom
{
	// Opens a new menu on the provided layer
	struct Menu
	{
		~Menu();

	protected:
		friend struct ptr<Menu>;
		Menu(std::shared_ptr<Control> control, std::vector<ptr<Option>> options);

	private:
		std::shared_ptr<Control> control;
		std::vector<ptr<Option>> options;
		ptr<Option> hovered;
	};


	inline Menu::Menu(std::shared_ptr<Control> control, std::vector<ptr<Option>> options) :
		control(control),
		options(options)
	{
		control->AddTask([this]()
		{
			if (Control::inputs[GLFW_MOUSE_BUTTON_2])
			{
				this->control.reset();
				return;
			};

			if (Control::inputs[GLFW_MOUSE_BUTTON_1] && hovered)
			{
				hovered->on_select();
				this->control.reset();
				return;
			};

			for (auto& option : this->options)
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
	inline Menu::~Menu()
	{
		
	};
};
