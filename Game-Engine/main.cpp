#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Game/Loader.h"

#include "Engine.h"
#include "Data.h"
#include "Text.h"
#include "Menu.h"
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
// TODO: Add special effects to sounds (i.e. reverb, 3D / 2D - ification, etc.)
// TODO: Implement a better compiler, then seperate .h and .cpp files
// TODO: Make Control work with ptr<>
// TODO: Remove floodfilling from map, move it to unit

int main()
{
	Engine::Init();
	
	auto map = LoadMap<1>();

	Engine::Run();

	Engine::Exit();

	return 0;
};
