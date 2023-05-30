#pragma once

#include "Data.h"
#include "Matrix.h"

#include <vector>

namespace loom
{
	struct DumbAI final : virtual protected Updateable, virtual protected Renderable
	{
		DumbAI(uint32_t irows, uint32_t icols, uint32_t orows, uint32_t ocols);

		void set_input(std::vector<std::vector<float*>>&& _input);

		Mat2D input;
		std::vector<Mat2D> operands;
		Mat2D output;

	private:
		void update() override;
		void render() override;
    };
};
