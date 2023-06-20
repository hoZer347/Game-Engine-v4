#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Loom.h"
#include "Testing.h"

#include "Engine.h"
#include "Camera.h"
#include "Data.h"
#include "Enums.h"
#include "Mesh.h"
#include "Control.h"
#include "Utils.h"
#include "Geometry.h"
using namespace loom;

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
// TODO: Add some sort of 


int main()
{
	Engine::Init();

	SpriteTest();

	ptr<Camera> camera;

	ptr<Control> control;
	Camera::InitiateFreeCam(*control);

	Rect rect;
	rect.trns *= translate(vec3(-1, -1, 0));

	Engine::Run();

	return 0;
};
