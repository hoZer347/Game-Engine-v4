#pragma once

#include "../Control.h"
#include "../TextMenu.h"
#include "../Menu.h"

#include "Map.h"
#include "MapImpl.h"
#include "Unit.h"

namespace loom
{
	struct MapControl
	{
		// Starts up the map and control system
		static void Init(MapImpl* map) { MapControl::map = map; ControlSelectATile(); };

		// Resets the control layer to when Init() was called
		static void Cancel(ptr<Unit> unit = ptr<Unit>());

		// Checks for tile selection
		static void ControlSelectATile();

		// Waits until a certain input is released
		static void WaitUntilUnpressed(int input, Task&& task);

		// When the player needs to select cell to send a unit
		static void ControlSelectUnitMove(ptr<Unit> unit);

		// When the player has to select an option after moving the unit
		static void ControlSelectAnOption(ptr<Unit> unit, Cell* destination);

		// Generate a list of options based on the capabilities of the given unit
		static std::vector<ptr<Option>> GetOptions(ptr<Unit> unit);

	private:
		MapControl() { };
		static inline MapImpl* map;
		static inline ptr<Menu> menu;
		static inline std::shared_ptr<Control> control;
	};


	inline void MapControl::Cancel(ptr<Unit> unit)
	{
		if (menu)
			menu.reset();

		if (control)
			control.reset();

		if (unit)
		{
			unit->pos = *unit->cell->pos;
			unit->cell->unit = unit;
		};

		map->clear_highlights();

		ControlSelectATile();
	};


	inline void MapControl::ControlSelectATile()
	{
		control = Control::next();

		Camera::InitiateFreeCam(control);

		control->AddTask([]()
		{
			map->find_hovered();

			if (Control::inputs[GLFW_MOUSE_BUTTON_1] && map->hovered)
				if (ptr<Unit> unit = map->hovered->unit)
					if (!unit->sprite->grayscale)
					{
						std::unordered_set<Cell*> _cells { map->hovered };

						if (unit->team == HIGHLIGHT::PLAYER)
						{
							map->clear_highlights();

							map->floodfill_hightlights(map->hovered->unit->team, 5, _cells);
							map->floodfill_hightlights(HIGHLIGHT::ATTACK_RANGE, 2, _cells);

							WaitUntilUnpressed(GLFW_MOUSE_BUTTON_1, [unit]() { ControlSelectUnitMove(unit); });
						}
						else map->floodfill_hightlights(map->hovered->unit->team, 7, _cells);
					};
		});
	};


	inline void MapControl::WaitUntilUnpressed(int input, Task&& task)
	{
		control = Control::next();

		Camera::InitiateFreeCam(control);

		control->AddTask([input, task]()
		{
			if (!Control::inputs[input])
				task();
		});
	};


	inline void MapControl::ControlSelectUnitMove(ptr<Unit> unit)
	{
		control = Control::next();

		Camera::InitiateFreeCam(control);

		control->Control::AddTask([unit]()
		{
			map->find_hovered();

			if (Cell* destination = map->hovered)
				if (destination->highlight == unit->team && !destination->unit)
					if (Control::inputs[GLFW_MOUSE_BUTTON_1])
						WaitUntilUnpressed(GLFW_MOUSE_BUTTON_1, [unit, destination]() { ControlSelectAnOption(unit, destination); });

			if (Control::inputs[GLFW_MOUSE_BUTTON_2])
				Cancel(unit);
		});
	};


	inline void MapControl::ControlSelectAnOption(ptr<Unit> unit, Cell* destination)
	{
		control = Control::next();
		
		menu = { 0, control, GetOptions(unit) };

		Camera::InitiateFreeCam(control);

		control->AddTask([unit, destination]()
		{
			Animate_MoveTowards(unit->pos, *destination->pos, .2f);

			if (Control::inputs[GLFW_MOUSE_BUTTON_2])
				Cancel(unit);
		});
	};


	inline std::vector<ptr<Option>> MapControl::GetOptions(ptr<Unit> unit)
	{
		std::vector<ptr<Option>> options;

		ptr<Font> font { 0, "Resources/alagard.ttf", 64 };

		vec3 offset{ 0, 0, 0 };

		// Attack
		options.push_back(TextOption({ 0, font, "Attack" }, offset += vec3(0, -1, 0), [unit]()
		{
			menu.reset();

			control = Control::next();

			Camera::InitiateFreeCam(control);

			control->AddTask([unit]()
			{
				map->find_hovered();

				if (map->hovered)
				{
					map->clear_highlights();

					std::unordered_set<Cell*> cells { map->hovered };
					map->floodfill_hightlights(HIGHLIGHT::ATTACK_RANGE, 3, cells);
				};

				if (Control::inputs[GLFW_MOUSE_BUTTON_2])
					Cancel(unit);
			});
		}));

		// Wait
		options.push_back(TextOption({ 0, font, "Wait" }, offset + vec3(0, -1, 0), [unit]()
		{
			unit->sprite->grayscale = true;

			unit->cell = map->hovered;

			map->hovered = nullptr;

			Cancel(unit);
		}));

		return options;
	};
};
