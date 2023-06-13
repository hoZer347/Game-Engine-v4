#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Game/Unit.h"
#include "Game/Map.h"
#include "Game/Cell.h"

#include "Camera.h"
#include "Data.h"
#include "Enums.h"
#include "Text.h"
#include "Input.h"
#include "Utils.h"
#include "Sprite.h"
#include "Geometry.h"
using namespace loom;

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include <iostream>

// TODO: Make sprites animate
// TODO: Make map highlights have an outline
// TODO: Fix load() functions to assign shader variables once only
// TODO: Make GameObject's destructor more efficient

int main()
{
	Loom::Init();

	Camera::InitiateFreeCam();

	Texture texture{ "Resources/Anna.png", GL_RGBA };
	Sprite sprite { texture, vec2{ 32 * 11, 32 * 11 }, vec2{ 32, 32 }, vec2{ 0, 0 }, 0 };

	Map map{ 4, 8 };
	GridOutline outline{ map };
	Highlights highlights{ map };
	map[1][1].highlight = 1;
	map[1][2].highlight = 1;
	map[2][2].highlight = 1;
	map[2][1].highlight = 1;

	Unit unit{ sprite, map[1][2] };

	Loom::Run();

	Loom::Exit();

	return 0;
};
