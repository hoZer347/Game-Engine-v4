#pragma once

#include "Data.h"

namespace loom
{
	struct Camera final : public Object, public Updatable
	{
		Camera();
		virtual ~Camera();

		mat4 mvp;

		float fovy = 90.f, aspect = 1, near = .1f, far = 1000.f;

	private:
		void load() override;
		void update() override;
		void unload() override;
	};
};
