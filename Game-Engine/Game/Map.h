#pragma once

#include "../Data.h"
#include "../Mesh.h"
#include "../Shader.h"
#include "../Camera.h"
#include "../Geometry.h"

namespace loom
{
	template <uint16 x_size, uint16 y_size>
	struct Map :
		virtual public Updateable
	{
		Map()
		{
			cells.reserve(x_size * y_size);


		};

		void update() override
		{ };
		void subRender() override
		{ };


		static inline mat4* mvp = &Camera::mvp;
		static inline vec4* color = new vec4{ 1, 0, 0, 1 };


		static inline Shader shader{ "Map", "mvp", &mvp, "color", &color };

		std::vector<Rect<4>> cells;
	};
};
