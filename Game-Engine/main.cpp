#include "GLEW/glew.h"
#include "GLFW/glfw3.h"


#include "AI.h"
#include "Camera.h"
#include "Data.h"
#include "Unit.h"
#include "Text.h"
#include "Input.h"
#include "Utils.h"
#include "Sprite.h"
#include "Commands.h"
#include "Physics.h"
#include "Grid.h"
#include "Geometry.h"
#include "Cuda.cuh"
#include "Matrix.h"
#include "Game/TestGame.h"
using namespace loom;


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;


#include <iostream>


int main()
{
	Loom::Init();


	Commands::InitiateFreeCam();

	Plane plane;


	Collision<Circle> circle {};
	OnCollisionFunction<Circle, Circle>([](Circle& p0, Circle& p1) { std::cout << "Test" << std::endl; });
	CollisionCompareFunction<Circle, Circle>([](Circle& p0, Circle& p1) { return true; });



	Loom::Run();

	Loom::Exit();

	return 0;
};
