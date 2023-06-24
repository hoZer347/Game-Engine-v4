#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Game/Map.h"
#include "Game/Unit.h"

#include "Loom.h"
#include "Data.h"
#include "Sprite.h"
#include "Engine.h"
using namespace loom;


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include <iostream>

// TODO: Make sprites animate
// TODO: Make GameObject's destructor more efficient
// TODO: Make a Shader uniform extractor
// TODO: Optimize sprites
// TODO: Add diagnostic timer
// TODO: Fix weird camera snapping glitch (most likely problems with double not being atomic)

int main()
{
	Engine::Init();

	Texture texture{ "Resources/Anna.png", GL_RGBA };
	Sprite sprite{ texture, vec2{ 5 * 32, 32 * 11 }, vec2{ 32, 32 }, vec2{ 0, 0 }, 0 };

	Camera camera;
	Control control;
	Camera::InitiateFreeCam(control);

	ptr<Map<500, 500>> map;
	map->highlights[4][4] = 1;
	map->refresh_highlights();
	map->terrain[5][5] = 0;
	map->terrain[6][5] = 0;
	map->terrain[6][6] = 0;
	map->terrain[5][6] = 0;
	map->refresh_grid();
	map->heights[3][3] = 1;
	map->heights[4][3] = 1;
	map->heights[4][4] = 1;
	map->heights[3][4] = 1;
	map->refresh_heights();

	Unit unit{ sprite };

	Engine::Add(&camera, &control, &map, &sprite, &unit);

	Engine::Run();

	return 0;
};
