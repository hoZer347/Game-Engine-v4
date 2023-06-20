#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

//#include "Game/Map.h"

#include "Loom.h"
#include "Testing.h"
#include "Data.h"
#include "Geometry.h"
#include "Sprite.h"
using namespace loom;
using namespace geo;


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include <iostream>

// TODO: Make sprites animate
// TODO: Make GameObject's destructor more efficient
// TODO: Make a Shader uniform extractor
// TODO: Make mesh do a big glBufferData first, and make the other stuff use glBufferSubData
// TODO: Hook up renderables to Mesh
// TODO: Completely rework Map
// TODO: Change loadable / unloadable to functional implementations


int main()
{
	Engine::Init();

	Camera camera;
	Control control;
	Camera::InitiateFreeCam(control);

	Rect<4> rect;
	rect.trns = translate(vec3(-1, -1, 0));
	Cube<4> cube;

	Engine::Add(&camera, &control, &rect, &cube);

	Engine::Run();

	return 0;
};
