#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include <string>

namespace loom
{
	struct Object;
	struct Renderable;
	struct Updatable;

	struct Loom final
	{
		static void Init();
		static void RunOnThisThread();
		static void Exit();

		static bool IsRunning();

	protected:
		friend struct Object;
		friend struct Renderable;
		friend struct Updatable;

		static void Add(Object* obj);
		static void Add(Renderable* obj);
		static void Add(Updatable* obj);

		static void Rmv(Object* obj);
		static void Rmv(Renderable* obj);
		static void Rmv(Updatable* obj);

	private:
		Loom() { };
	};
};
