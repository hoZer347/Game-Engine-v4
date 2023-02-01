#pragma once

#include "Data.h"
#include "Utils.h"

#include <glm/glm.hpp>
using namespace glm;

#include <memory>

namespace loom
{
	struct Cell final { vec4 pos = vec4(0); };

	struct Grid final
	{
		Grid(uint16_t x_size, uint16_t y_size);
		virtual ~Grid();

		mat4 trns = mat4(1);

	protected:
		friend struct Utils;
		void load();
		void render();

	private:
		struct Column
		{
			Cell* ptr;
			Cell& operator[](int i) { return ptr[i]; }
		};
		
		std::unique_ptr<Cell[]> vtxs;
		uint16_t x_size, y_size;
		uint32_t _trns = 0, _mvp = 0;
		std::vector<uint32_t> inds;
		static inline std::shared_ptr<Shader> shader = Utils::Construct<Shader>("shaders/Grid");

	public:
		Column operator[](uint16_t i) { return Column({ &vtxs[i * x_size] }); }
	};
};
