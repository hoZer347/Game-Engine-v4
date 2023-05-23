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
#include "Grid.h"
#include "Cuda.cuh"
#include "Matrix.h"
using namespace loom;

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include <iostream>

// TODOS: //
// 
// Shader / Texture
// - Make shaders auto-generate files
// 
// GameObject
// - Make game objects auto-generate based on their settings
// 
// Inputs
// - Turn inputs into a singleton that has many input layers
// - Rework to make more intuitive
// ex: load(std::string state)
// 
// Text
// - Finish the text engine
// 
// Sound
// - Start the sound engine
// 
// Add Menu
// - Add menus
// 
// Editors to add:
// - Grid
// - Sprite
// - Unit
// 
// 
// Add Generic Error Handling
// 
// TODOS: //

int main()
{
	Loom::Init();


	// Sprite
	Texture texture{ "Resources/Anna.png", GL_RGBA };
	Sprite sprite{ texture, { 0, 320 }, { 32, 32 }, { 0, 0 }, 0 };

	// Text
	Font font{ "Resources/alagard.ttf" };
	DynamicText text{ font, "Test" };


	// Grid
	Grid grid { 10, 10 };
	Commands::InitiateFreeCam();


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
