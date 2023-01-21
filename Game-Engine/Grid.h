#pragma once

#include "Data.h"

namespace loom
{
	struct Cell final { vec4 pos = vec4(0); };

	struct Grid final : public Renderable
	{
		Grid(uint16_t x_size, uint16_t y_size);
		virtual ~Grid();

		mat4 trns = mat4(1);

	private:
		struct Column
		{
			Cell* ptr;
			Cell& operator[](int i) { return ptr[i]; }
		};

		void load() override;
		void render() override;
		void unload() override;

		std::unique_ptr<Cell[]> vtxs;
		uint16_t x_size, y_size;
		uint32_t _trns = 0, _mvp = 0;
		std::vector<uint32_t> inds;
		static inline Shader shader{ "shaders/Grid"};

	public:
		Column operator[](int i) { return Column({ &vtxs[i * x_size] }); }
	};
};
