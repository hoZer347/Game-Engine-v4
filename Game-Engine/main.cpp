#include "GLEW/glew.h"
#include "GLFW/glfw3.h"


#include "Game/Grid.h"
#include "Game/Unit.h"
#include "Game/TestGame.h"


#include "AI.h"
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


int main()
{
	Loom::Init();


	Camera::InitiateFreeCam();

	Font font{ "Resources/alagard.ttf", 72 };
	DynamicText text{ font, "Test" };


	Loom::Run();

	Loom::Exit();

	return 0;
};
