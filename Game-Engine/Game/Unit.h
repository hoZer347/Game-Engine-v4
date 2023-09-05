#pragma once

#include "../Camera.h"
#include "../Sprite.h"

#include "Cell.h"

#include <unordered_set>

namespace loom
{
	typedef void(*FloodFillFunc)(Unit* origin, std::unordered_set<Cell*> cells);

	// Highlights the range in which a unit is allowed to move on their turn
	void PlayerPhaseStandardFloodfill(Unit* origin, std::unordered_set<Cell*> cells);
	
	// Highlights an attack range in which a unit can attack
	void AttackRangeStandardFloodfill(uint8 range, std::unordered_set<Cell*> cells);

	// Stat spread of a given unit
	struct alignas(8) StatSpread final
	{
		uint8
			HP	= 0,
			STR = 0,
			DEX = 0,
			INT = 0,
			WIS = 0,
			DEF = 0,
			RES = 0,
			MOV = 5;

		StatSpread* next = nullptr;
	};

	struct Unit final :
		GameObject<Unit>,
		virtual public Updateable
	{
		vec4 pos{ 0, 0, 0, 1 };
		Cell* cell = nullptr;
		uint8 team = HIGHLIGHT::ENEMY;
		bool& done = sprite->grayscale;
		ptr<Sprite> sprite;
		StatSpread stats;

	protected:
		friend struct ptr<Unit>;
		Unit(ptr<Sprite> sprite) :
			sprite(sprite)
		{ };

	private:
		void update() override
		{
			mat4 _trns = translate(vec3(pos) + vec3(.5, .5, 0));
			_trns *= Camera::yaww_mat;
			_trns *= translate(vec3(-.5, 0, 0));

			sprite->trns = _trns;
		};
	};


	void PlayerPhaseStandardFloodfill(Unit* origin, std::unordered_set<Cell*> cells)
	{
		if (cells.size() == 1)
			(*cells.begin())->highlight = origin->stats.MOV;

		// Movement Range
		uint8 amount = origin->stats.MOV;

		for (; amount > 0; amount--)
		{
			auto _cells = cells;

			for (auto& _cell : _cells)
				if (_cell)
				{
					cells.insert(_cell->U);
					cells.insert(_cell->D);
					cells.insert(_cell->L);
					cells.insert(_cell->R);
				};
		};

		for (auto& cell : cells)
			cell->highlight = origin->team;
		//
	};

	void AttackRangeStandardFloodfill(uint8 range, std::unordered_set<Cell*> cells)
	{
		for (; range > 0; range--)
		{
			auto _cells = cells;

			for (auto& _cell : _cells)
				if (_cell)
				{
					cells.insert(_cell->U);
					cells.insert(_cell->D);
					cells.insert(_cell->L);
					cells.insert(_cell->R);
				};
		};

		for (auto& cell : cells)
			cell->highlight = HIGHLIGHT::ATTACK_RANGE;
	};
};
