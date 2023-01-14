#pragma once

#include "Data.h"

namespace loom
{
	struct Camera final : public Updatable
	{
		static inline mat4 mvp;

	private:
		mat4	mode,
				view,
				proj;
		
		vec4	pos;

		vec3	eye,
				ctr,
				up;

		float	fovy=0,
				aspect=0,
				near=0,
				far=0;

	private:
		void load() override;
		void update() override;
		void unload() override;
	};
};
