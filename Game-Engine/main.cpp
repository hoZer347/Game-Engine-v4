#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Game/Unit.h"
#include "Game/Map.h"

#include "Camera.h"
#include "Data.h"
#include "Enums.h"
#include "Text.h"
#include "Input.h"
#include "Utils.h"
#include "Sprite.h"
#include "Commands.h"
#include "Geometry.h"
#include "Cuda.cuh"
#include "Matrix.h"
#include "vulkan/vulkan.h"
using namespace loom;

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include <iostream>


int main()
{
	Loom::Init();

	Camera::InitiateFreeCam();

	Texture texture{ "Resources/Anna.png", GL_RGBA };
	Geometry<Sprite> sprite { texture, vec2{ 32 * 11, 32 * 11 }, vec2{ 32, 32 }, vec2{ 0, 0 }, 60 };

	Map map{ 4, 8 };
	GridOutline outline{ map };
	Highlights highlights{ map };
	map[1][1].highlight = 1;
	map[1][2].highlight = 1;
	map[2][2].highlight = 1;
	map[2][1].highlight = 1;

	Inputs::AddInput([]()
	{
		static mat4 m = mat4(1);
		Square* square = new Square();
		square->trns *= m *= translate(vec3(0, 0.1, 0.1));

	}, { GLFW_KEY_Q, 255, GLFW_PRESS, 0 });

	Loom::Run();

	Loom::Exit();

	return 0;
};
