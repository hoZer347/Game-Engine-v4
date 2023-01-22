#pragma once

#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	template <typename S, typename T>
	struct approach_static
	{
		approach_static(S& _start, const T& _finish, const float& velocity = 1)
		: _start(_start),
		  _finish(_finish)
		{ _move = normalize(_finish - _start) * velocity; };
		
		operator bool()
		{
			bool ret = true;

			for (int i = 0; i < minSize; i++)
				if (_finish[i] - _start[i] <= _move[i])
					_start[i] = _finish[i];
				else
				{
					_start[i] += _move[i];
					ret = false;
				};

			return ret;
		};

		static inline const int minSize = min((int)sizeof(S) / sizeof(float), (int)sizeof(T) / sizeof(float));

	private:
		vec4 _move;
		      S& _start;
		const T& _finish;
	};
};
