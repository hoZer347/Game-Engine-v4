#include "Loader.h"

#include "../Data.h"
#include "Map.h"
#include "MapControl.h"
#include "Unit.h"
#include "../Sound.h"
#include "../Camera.h"

namespace loom
{
	template <>
	ptr<Container> LoadMap<1>()
	{
		ptr<Texture> texture{ 1, "Resources/Anna.png", GL_RGBA };
		ptr<Sprite> sprite0{ 1, texture, vec2{ 5 * 32, 32 * 11 }, vec2{ 32, 32 }, vec2{ 0, 0 }, 0 };
		ptr<Unit> unit0{ 1, sprite0 };
		unit0->team = HIGHLIGHT::PLAYER;

		ptr<Sprite> sprite1{ 1, texture, vec2{ 0 * 32, 32 * 11 }, vec2{ 32, 32 }, vec2{ 0, 0 }, 0 };
		ptr<Unit> unit1{ 1, sprite1 };
		unit1->team = HIGHLIGHT::ENEMY;

		ptr<Sprite> sprite2{ 1, texture, vec2{ 0 * 32, 32 * 11 }, vec2{ 32, 32 }, vec2{ 0, 0 }, 0 };
		ptr<Unit> unit2{ 1, sprite2 };
		unit2->team = HIGHLIGHT::ENEMY;

		ptr<Map<32, 32, 4>> map { 1 };

		map->connect_adjacent(0, 0, 32, 32);
		map->generate();

		map->set_unit(unit0, 5, 5);
		map->set_unit(unit1, 6, 5);
		map->set_unit(unit2, 5, 6);

		glBindTexture(GL_TEXTURE_2D, map->texture->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		map->height_func = [](auto x, auto y, const uint64& time_elapsed)
		{
			return 2 + (cos(x + (float)time_elapsed / 100) + cos(y + (float)time_elapsed / 100)) / 5;
		};

		MapControl::Init(map.get());

		return ptr<Container>(1, unit0, unit1, unit2, map);
	};

	template <>
	ptr<Container> LoadScene<1>()
	{


		return ptr<Container>(1);
	};
};
