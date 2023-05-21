#include "AI.h"

namespace loom
{
	DumbAI::DumbAI(
		uint32_t irows, uint32_t icols,
		uint32_t orows, uint32_t ocols) :
		input(irows, icols),
		output(orows, ocols)
	{ };
	void DumbAI::set_input(std::vector<std::vector<float*>>&& _input)
	{
		for (uint32_t i = 0; i < input.cols; i++)
			for (uint32_t j = 0; j < input.rows; j++)
				input[i][j] = *_input[i][j];
	};
	void DumbAI::update()
	{
		
	};
	void DumbAI::render()
	{
		Mat2D::multiply(input, output, output);

		for (auto& mat : operands)
			Mat2D::multiply(mat, output, output);
	};
};
