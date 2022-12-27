// Local Includes
#include "Rendering.h"
#include "Engine.h"
#include "Buffer.h"
#include "Camera.h"
#include "Task.h"
#include "Data.h"
using namespace loom;
//

// OpenGL Includes
#include "GLFW/glfw3.h"
#include "glm/gtx/transform.hpp"
using namespace glm;
//

// STD includes
#include <iostream>
//

int main()
{
	init();

	Buffer<Mesh> b0;
	Camera cam;

	Thread* t0 = create_thread();
	
	open_window(t0);

	assign_on_init(t0, geo::_create_squares(b0, 100));

	assign(t0, cam._update());
	assign(t0, b0._update());
	assign(t0, _render(b0));

	run();

	finish();

	return 0;
};
