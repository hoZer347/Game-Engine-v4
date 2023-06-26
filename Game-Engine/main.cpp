#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Game/Map.h"
#include "Game/Unit.h"
#include "Game/Terrain.h"

#include "Loom.h"
#include "Data.h"
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
// TODO: Optimize sprites
// TODO: Add diagnostic timer

int main()
{
	Engine::Init();

	Texture texture{ "Resources/Anna.png", GL_RGBA };

	Sprite sprite{ texture, vec2{ 5 * 32, 32 * 11 }, vec2{ 32, 32 }, vec2{ 0, 0 }, 0 };
	Unit unit{ sprite };

	Camera::InitiateFreeCam();

	ptr<Map<32, 32, 4, 4>> map;
	map->set_unit(&unit, 5, 5);
	map->connect_adjacent(0, 0, 32, 32);
	map->floodfill_hightlights(HIGHLIGHT::PLAYER, 5, map->get_cell_ptr(0, 0));
	map->generate();

	Engine::Add(&unit, &sprite);

	Engine::Run();

	return 0;
};
