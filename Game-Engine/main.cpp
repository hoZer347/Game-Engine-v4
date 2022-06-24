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
#include <thread>
//

// Local Includes
#include "Engine.h"
#include "Window.h"
//

inline unsigned int i = 0;

DEFINE_TASK(Task, eng::Thread, )
{
	std::cout << ++i << std::endl;

	delete this;
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	eng::init();
	
	auto w0 = eng::wnd::create();

	w0->init();

	while (!w0->KILL)
	{
		w0->assign(Task::create(w0.get()).release());

		std::this_thread::sleep_for(std::chrono::seconds(1));
	};

	w0->join();

	w0.reset();

	eng::close();

	_CrtDumpMemoryLeaks();
	return 0;
};
