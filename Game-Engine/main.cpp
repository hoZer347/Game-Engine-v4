#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Game/Unit.h"
#include "Game/Map.h"
#include "Game/Cell.h"

#include "Camera.h"
#include "Data.h"
#include "Enums.h"
#include "Text.h"
#include "Mesh.h"
#include "Control.h"
#include "Utils.h"
#include "Menu.h"
#include "Sprite.h"
#include "Geometry.h"
using namespace loom;

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include <iostream>

// TODO: Make sprites animate
// TODO: Make map highlights have an outline
// TODO: Make GameObject's destructor more efficient
// TODO: Make a Shader uniform extractor
// TODO: Do a big glBufferData first, and make the other stuff use glBufferSubData
// TODO: Hook up renderables to Mesh

int main()
{
	Engine::Init();

	Camera camera;

	Control control;
	Camera::InitiateFreeCam(control);

	Texture texture{ "Resources/Anna.png", GL_RGBA };
	Sprite sprite{ texture, vec2{ 5 * 32, 32 * 11 }, vec2{ 32, 32 }, vec2{ 0, 0 }, 0 };

	Map map{ 4, 8 };
	GridOutline outline{ map };
	Highlights highlights{ map };
	map[1][1].highlight = 1;
	map[1][2].highlight = 1;
	map[2][2].highlight = 1;
	map[2][1].highlight = 1;

	Unit unit{ sprite, map[1][2] };

	Shader shader{ "Geometry" };
	Mesh3D<4> mesh{ shader, GL_QUADS, 4, 4 };
	mesh.mvp = &Camera::vp;
	mesh.allocate(0,
		vec4(0, 0, -10, 1),
		vec4(1, 0, -10, 1),
		vec4(1, 1, -10, 1),
		vec4(0, 1, -10, 1));
	mesh.index(0, 0, 1, 2, 3);

	Engine::Add(
		&control,
		&texture,
		&sprite,
		&map,
		&outline,
		&highlights);

	Engine::Run();

	return 0;
};
