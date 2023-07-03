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

int main()
{
	Engine::Init();
	
	LoadMap<1>();

	Engine::Exit();

	return 0;
};
