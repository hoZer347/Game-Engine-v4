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

//

// Local Includes
#include "Window.h"
#include "Engine.h"
#include "Object.h"
#include "Inputs.h"
#include "Stages.hpp"
//

#include <thread>
#include <iostream>

class test_obj : obj::Obj
{
	void exec(obj::Stage& stage) override
	{
		switch (stage)
		{
		case 0:
			std::cout << "Test1" << std::endl;
			break;
		case 1:
			std::cout << "Test2" << std::endl;
			break;
		case 2:
			std::cout << "Test3" << std::endl;
			break;
		};
	};
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	test_obj();
	test_obj();
	test_obj();
	test_obj();

	auto e = eng::create();
	auto w = win::create(e);
	eng::assign_tasks(e, { STG_UPDATE, STG_RENDER });
	eng::start(e);
	eng::wait(e);
	win::del(w);

	_CrtDumpMemoryLeaks();

	return 0;
};
