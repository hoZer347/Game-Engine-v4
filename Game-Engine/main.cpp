#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "AI.h"
#include "Camera.h"
#include "Data.h"
#include "Input.h"
#include "Utils.h"
#include "Sprite.h"
#include "Commands.h"
#include "Grid.h"
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


	// Sprite
	Texture texture{ "Resources/Anna.png", GL_RGBA };
	Sprite sprite{ texture, { 0, 320 }, { 32, 32 }, { 0, 0 }, 0 };


	// Grid
	Grid grid { 10, 10 };
	grid.ConstructCells(0, 0, 10, 10);
	Commands::bindCameraToGrid(grid);


	// Matrix Multiplication
	Mat2D m0{ 2, 3, Mat2D::_gen_value_is_index };
	Mat2D m1{ 3, 2, Mat2D::_gen_value_is_index };
	Mat2D m2{ 2, 2 };
	Mat2D::multiply(m0, m1, m2);
	m0.print();
	m1.print();
	m2.print();


	Loom::Run();

	Loom::Exit();

	return 0;
};
