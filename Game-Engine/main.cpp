// Local Includes
#define NUMBER_OF_THREADS 2
#define RENDERABLE_IMPL
#include "Engine.h"
#include "Window.h"
#include "Game.h"
#include "Data.h"
using namespace loom;
//

// STD includes
#include <iostream>
//

int main()
{
	auto w0 = Window::create();

	System<int, (size_t)0>::allocate(5);

	w0->join();

	return 0;
};
