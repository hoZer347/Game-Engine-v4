#pragma once

#include "Text.h"
#include "Option.h"

#include "glm/glm.hpp"
#include "glm/gtx/intersect.hpp"
using namespace glm;

namespace loom
{
	ptr<Option> TextOption(const ptr<DynamicText> text, vec3 offset, Task on_select)
	{
		text->trns *= translate(offset);
		text->color = vec4(.3, .3, .3, 1);
		return ptr<Option>
		{
			1,
			[text]()
			{
				text->color = vec4(0, 0, 0, 1);
			},
			[text]()
			{
				text->color = vec4(.3, .3, .3, 1);
			},
			on_select,
			[text]()
			{
				vec4 pos0 = text->trns * vec4(0, 0, 0, 1);
				vec4 pos1 = text->trns * vec4(text->curr_pos.x, 0, 0, 1);
				vec4 pos2 = text->trns * vec4(text->curr_pos.x, text->curr_pos.y + 1, 0, 1);
				vec4 pos3 = text->trns * vec4(0, text->curr_pos.y + 1, 0, 1);

				vec2 buh;
				float f;
				if (intersectRayTriangle(
					vec3(Camera::mpos),
					vec3(Camera::mdir),
					vec3(pos0),
					vec3(pos1),
					vec3(pos2),
					buh,
					f) ||
					intersectRayTriangle(
					vec3(Camera::mpos),
					vec3(Camera::mdir),
					vec3(pos0),
					vec3(pos3),
					vec3(pos2),
					buh,
					f))
					return true;

				return false;
			}
		};
	};
};
