// Debug
#include "Mem_Leak.h"
//

// OpenGL
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;
//

// std includes
#include <iostream>
//

// Local Includes
#include "Game.h"
#include "Objects.h"
#include "Shader.h"
#include "Mesh.h"
#include "Threadable.h"
//

#include <iostream>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game* g = new Game();

	g->init();
	g->wait();

	delete g;

	_CrtDumpMemoryLeaks();

	return 0;
};
