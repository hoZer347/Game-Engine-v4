#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Game/Map.h"
#include "Game/MapControl.h"
#include "Game/Unit.h"

#include "Loom.h"
#include "Data.h"
#include "Menu.h"
#include "Sprite.h"
#include "Engine.h"
using namespace loom;

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include <iostream>
#include <random>

// TODO: Make sprites animate
// TODO: Make GameObject's destructor more efficient
// TODO: Make a Shader uniform extractor
// TODO: Optimize sprites (do one draw call per texture)
// TODO: Add diagnostic timer

int main()
{
	Engine::Init();

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

	ptr<Map<16, 32>> map { 1 };

	map->connect_adjacent(0, 0, 32, 32);
	map->generate();

	map->set_unit(unit0, 5, 5);
	map->set_unit(unit1, 6, 5);
	map->set_unit(unit2, 5, 6);

	MapControl::Init(map.get());

	Engine::Run();

	return 0;
};
