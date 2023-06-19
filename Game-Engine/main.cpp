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
// TODO: Make mesh do a big glBufferData first, and make the other stuff use glBufferSubData
// TODO: Hook up renderables to Mesh
// TODO: Completely rework Map

int main()
{
	Engine::Init();

	ptr<Camera> camera;

	ptr<Control> control;
	Camera::InitiateFreeCam(*control);

	Texture texture{ "Resources/Anna.png", GL_RGBA };
	Sprite sprite{ texture, vec2{ 5 * 32, 32 * 11 }, vec2{ 32, 32 }, vec2{ 0, 0 }, 0 };
	sprite.trns = translate(vec3(0));
	
	ptr<Cube> cube;

	Engine::Add(&texture, &sprite);

	Engine::Run();

	return 0;
};
