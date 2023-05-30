#pragma once

#include "Data.h"
#include <glm/glm.hpp>
using namespace glm;

#include <vector>

namespace loom
{
	struct _Geo
	{
		_Geo(mat4& trns, std::vector<vec4>& vtxs) :
			trns(trns),
			vtxs(vtxs)
		{ };

		mat4& trns;
		std::vector<vec4>& vtxs;

		vec3 pos = vec3(0), size = vec3(0);
	};


	template <typename T>
	struct Geo final : public _Geo
	{
		template <typename... ARGS>
		Geo(ARGS... args) :
			obj(args...),
			_Geo(obj.trns, obj.vtxs)
		{ };

		T obj;
	};


	struct Cube final :
		virtual private Loadable,
		virtual private Renderable
	{
		vec4 color = vec4(0, 0, 1, 1);
		mat4 trns = mat4(1);

	private:
		void load() override;
		void render() override;

		static inline std::vector<vec4> vtxs =
		{
			vec4(0, 0, 0, 1),
			vec4(1, 0, 0, 1),
			vec4(1, 1, 0, 1),
			vec4(0, 1, 0, 1),

			vec4(0, 0, 1, 1),
			vec4(1, 0, 1, 1),
			vec4(1, 1, 1, 1),
			vec4(0, 1, 1, 1),
		};
		static inline std::vector<uint32_t> inds =
		{
			0, 1, 2, 3,
			7, 6, 5, 4,
			1, 5, 6, 2,
			4, 0, 3, 7,
			0, 1, 5, 4,
			3, 2, 6, 7,
		};
	};


	struct Plane final :
		virtual private Loadable,
		virtual private Renderable
	{
		vec4 color = vec4(1);
		mat4 trns = mat4(1);

	private:
		void load() override;
		void render() override;

	protected:
		friend struct Geo<Plane>;

		static inline std::vector<vec4> vtxs =
		{
			vec4(0, 0, 0, 1),
			vec4(1, 0, 0, 1),
			vec4(1, 1, 0, 1),
			vec4(0, 1, 0, 1),
		};
	};


	struct Circle final :
		virtual private Loadable,
		virtual private Renderable
	{
		const float radius = 1.0f;

		vec4 color = vec4(1);
		mat4 trns = mat4(1);

	private:
		void load() override;
		void render() override;
	};
};
