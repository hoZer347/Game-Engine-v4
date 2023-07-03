#pragma once

#include "Text.h"
#include "Option.h"

#include "glm/glm.hpp"
#include "glm/gtx/intersect.hpp"
using namespace glm;

namespace loom
{
	ptr<Option> TextOption(const ptr<DynamicText> text, float offset, Task on_select)
	{
		text->color = vec4(.3, .3, .3, 1);
		return ptr<Option>
		{
			1,
			[text]()
			{
				text->color = vec4(0, 0, 0, 1);
				std::cout << "On" << std::endl;
			},
			[text]()
			{
				text->color = vec4(.3, .3, .3, 1);
				std::cout << "Off" << std::endl;
			},
			on_select,
			[text]()
			{
				vec2 buh;
				float f;
				if (intersectRayTriangle(
					vec3(Camera::mpos),
					vec3(Camera::mdir),
					vec3(vec4(0, 0, 0, 1)),
					vec3(vec4(1, 0, 0, 1)),
					vec3(vec4(1, 1, 0, 1)),
					buh,
					f) ||
					intersectRayTriangle(
					vec3(Camera::mpos),
					vec3(Camera::mdir),
					vec3(vec4(0, 0, 0, 1)),
					vec3(vec4(0, 1, 0, 1)),
					vec3(vec4(1, 1, 0, 1)),
					buh,
					f))
					return true;

				return false;
			}
		};
	};
};
