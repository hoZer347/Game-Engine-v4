// Debug
#define DEBUG
//

// OpenGL
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
using namespace glm;
//

// STD includes
#include <iostream>
//

// Local Includes
#include "Engine.h"
#include "Renderables.h"
#include "Data.h"
using namespace eng;
//

// STB Image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//

int main()
{
	start();

	Data<int>::create();
	Data<int>::create(50);

	close();

	return 0;
};
