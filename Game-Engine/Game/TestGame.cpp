#include "TestGame.h"

#include <GLEW/glew.h>
#include <glm/gtx/transform.hpp>

namespace loom
{
	TestGame::TestGame()
	{
		arena.trns *= scale(vec3(1.5, 1, 1) * arena_size);

		p0.trns *= translate(vec3(arena_size * 1.5 * .1 - .5, 1 * arena_size / 2 - .5, .001));
		p1.trns *= translate(vec3(arena_size * 1.5 * .3 - .5, 1 * arena_size / 2 - .5, .001));
		p2.trns *= translate(vec3(arena_size * 1.5 * .7 - .5, 1 * arena_size / 2 - .5, .001));
		p3.trns *= translate(vec3(arena_size * 1.5 * .9 - .5, 1 * arena_size / 2 - .5, .001));
		ball.trns *= translate(vec3(1.5 * arena_size / 2 - .5, 1 * arena_size / 2 - .5, .001));
<<<<<<< HEAD

=======
		
>>>>>>> 7b78e568609d687c3cb3fb403fe3d5dfab06d4b9
		goal0.trns *= scale(vec3(1, 1 * (arena_size / 3), 0));
		goal1.trns *= scale(vec3(1, 1 * (arena_size / 3), 0));
		goal0.trns *= translate(vec3(-1, 1, 0));
		goal1.trns *= translate(vec3(1.5 * arena_size, 1, 0));

		p0.color = vec4(1, 0, 0, 1);
		p1.color = vec4(1, 0, 0, 1);
		goal0.color = vec4(1, 0, 0, 1);

		p2.color = vec4(0, 0, 1, 1);
		p3.color = vec4(0, 0, 1, 1);
		goal1.color = vec4(0, 0, 1, 1);

		arena.color = vec4(0, .8, 0, 1);
	};
	TestGame::~TestGame()
	{

	};
	void TestGame::update()
	{
<<<<<<< HEAD

	};
	void TestGame::render()
	{

=======
		
	};
	void TestGame::render()
	{
		
>>>>>>> 7b78e568609d687c3cb3fb403fe3d5dfab06d4b9
	};
};
