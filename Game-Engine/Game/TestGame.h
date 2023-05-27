#pragma once

#include "../Data.h"
#include "../Geometry.h"

#ifndef MAX_TEST_GAME_SPEED
#define MAX_TEST_GAME_SPEED 1
#endif

namespace loom
{
	struct TestGame final :
		virtual protected Updateable,
		virtual protected Renderable
	{
		TestGame();
		~TestGame();

	private:
		void update() override;
		void render() override;

		float arena_size = 15;

		Plane arena, p0, p1, p2, p3, goal0, goal1, ball;

		static inline Shader shader{ "Game/TestGameShader" };
	};
};
