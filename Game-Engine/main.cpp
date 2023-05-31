#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Game/Grid.h"
#include "Game/Unit.h"
#include "Game/TestGame.h"

#include "Camera.h"
#include "Data.h"
#include "Text.h"
#include "Input.h"
#include "Utils.h"
#include "Sprite.h"
#include "Commands.h"
#include "Physics.h"
#include "Geometry.h"
#include "Cuda.cuh"
#include "Matrix.h"
using namespace loom;

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include <iostream>
#include <type_traits>


int main()
{
	Loom::Init();

	Camera::InitiateFreeCam();

	Font font{ "Resources/Vogue.ttf", 72 };
	DynamicText text{ font, "Test" };

	Texture texture{ "Resources/Anna.png", GL_RGBA };
	Geometry<Sprite> sprite { texture, vec2{ 32 * 10, 32 * 10 }, vec2{ 32, 32 }, vec2{ 0, 0 }, 60 };

	Square square;
	
	Mount(&font, &text, &texture, &sprite, &square);

	Loom::Run();

	Loom::Exit();

	return 0;
};
