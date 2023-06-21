#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

//#include "Game/Map.h"

#include "Loom.h"
#include "Testing.h"
#include "Data.h"
#include "Geometry.h"
#include "Sprite.h"
using namespace loom;


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include <iostream>


// TODO: Make sprites animate
// TODO: Make GameObject's destructor more efficient
// TODO: Make a Shader uniform extractor
// TODO: Make mesh do a big glBufferData first, and make the other stuff use glBufferSubData
// TODO: Hook up renderables to Mesh
// TODO: Completely rework Map
// TODO: Change loadable / unloadable to functional implementations
// TODO: Optimize sprites
// TODO: Add diagnostic timer


void test(std::vector<int>&& v, int&& i, float&& f)
{

};

int main()
{
	Engine::Init();

	Camera camera;
	Control control;
	Camera::InitiateFreeCam(control);

	Texture texture{ "Resources/Anna.png", GL_RGBA };
	sprite::Sprite sprite{ texture, vec2{ 5 * 32, 32 * 11 }, vec2{ 32, 32 }, vec2{ 0, 0 }, 0 };

	Engine::Add(&texture, &sprite, &camera, &control);

	geo::Rect rect0;
	rect0.trns = translate(vec3(-1, -1, 0));
	geo::Rect rect1;

	Engine::Add(&rect0, &rect1);

	Engine::Run();

	return 0;
};
