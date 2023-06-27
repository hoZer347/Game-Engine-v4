#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Game/Map.h"
#include "Game/Unit.h"
#include "Game/Terrain.h"

#include "Loom.h"
#include "Data.h"
#include "Menu.h"
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

	ptr<Texture> texture{ 1, "Resources/Anna.png", GL_RGBA };

	ptr<Sprite> sprite{ 1, texture, vec2{ 5 * 32, 32 * 11 }, vec2{ 32, 32 }, vec2{ 0, 0 }, 0 };
	ptr<Unit> unit{ 1, sprite };

	Camera::InitiateFreeCam();

	ptr<Map<32, 32, 4, 4>> map { 1 };
	map->set_unit(unit, 5, 5);
	map->connect_adjacent(0, 0, 32, 32);
	map->floodfill_hightlights(HIGHLIGHT::PLAYER, 5, map->get_cell_ptr(0, 0));
	map->generate();

	CheckForPressed<GLFW_KEY_ESCAPE> option0;
	option0.on_select = []() { Menu::leave(); };

	CheckForPressed<GLFW_KEY_Q> option1;
	option1.on_hover = []() { std::cout << "Q" << std::endl; };

	CheckForPressed<GLFW_KEY_W> option2;
	option2.on_hover = []() { std::cout << "W" << std::endl; };

	Menu::next({ option0, option1, option2 }, []() { return true; });



	Engine::Run();

	return 0;
};
