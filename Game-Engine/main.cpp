// Local Includes
#define NUMBER_OF_THREADS 2
#define RENDERABLE_IMPL
#include "Barrier.h"
#include "Engine.h"
#include "Buffer.h"
#include "Camera.h"
#include "Task.h"
#include "Data.h"

#include "GLFW/glfw3.h"
#include "glm/gtx/transform.hpp"
using namespace glm;
//

using namespace loom;

// STD includes
#include <iostream>
//

int main()
{
	init();

	Buffer<Renderable> b0;
	Camera cam;
	b0._allocate(1)();
	b0._update()();
	b0[0].meshes.push_back({
		{	{ vec4(-1, -1, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(0, 0, 0, 1), },
			{ vec4( 1, -1, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 0, 0, 1), },
			{ vec4( 1,  1, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 1, 0, 1), },
			{ vec4(-1,  1, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(0, 1, 0, 1), }},
		{ 0, 1, 2, 3 } });

	Thread* t0 = create_thread();
	Thread* t1 = create_thread();

	open_window(t0);
	assign(t0, _render_func(b0));
	assign(t0, cam._update());
	assign_on_init(t0, _load_shader(b0[0], "shaders/default"));
	assign_on_init(t0, _load_textures(b0[0], {{ "Resources/stone.png", GL_RGB }}));
	sync({ t0 }, Barrier::create<1, 0>(), b0._update());

	run();

	return 0;
};
