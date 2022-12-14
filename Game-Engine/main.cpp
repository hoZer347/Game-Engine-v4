// Local Includes
#define NUMBER_OF_THREADS 2
#define RENDERABLE_IMPL
#include "Engine.h"
#include "Buffer.h"
#include "Sync.h"
#include "Data.h"

#include "GLFW/glfw3.h"

using namespace loom;
//

// STD includes
#include <iostream>
//

int main()
{
	init();
	
	Thread* w0 = create_thread();
	open_window(w0);

	Thread* t0 = create_thread();
	Thread* t1 = create_thread();

	Task edit = Buffer<int, 0>::_edit([](int& i)
	{
		i++;
	});
	Task view = Buffer<int, 0>::_view([](const int& i)
	{
		std::cout << i << std::endl;
	});
	Task aloc = Buffer<int, 0>::_allocate(5, 0);

	assign_on_init(t0, aloc);

	assign(t0, edit);
	assign(t1, edit);

	assign(w0, view);
	assign(w0, []() noexcept
	{
		if (GLFWwindow* window = glfwGetCurrentContext())
			if (glfwWindowShouldClose(window))
				close();
	});

	Sync<0>::align<3>({ w0, t0, t1 }, Buffer<int, 0>::_swap());

	run({ w0, t0, t1 });

	return 0;
};
