#include "Matrix.h"
#include "Cuda.cuh"

#include <iostream>

namespace loom
{
	Mat2D::Mat2D(uint32_t rows, uint32_t cols,
		void(*_gen)(float*, const uint32_t&, const uint32_t&)) :
		rows(rows), cols(cols),
		data((float*)malloc(rows * cols * sizeof(float)))
	{ if (_gen) _gen(data, rows, cols); };
	Mat2D::~Mat2D()
	{ free(data); }
	void Mat2D::transpose(Mat2D& result)
	{
		if (result.rows != cols || result.cols != rows)
			throw std::invalid_argument("Invalid matrix dimensions for transpose");

		for (uint32_t i = 0; i < rows; ++i)
			for (uint32_t j = 0; j < cols; ++j)
				result[j][i] = data[i * cols + j];
	};
	Mat2D& Mat2D::operator=(const Mat2D& other)
	{
		if (this == &other)
			return *this;

		if (rows != other.rows || cols != other.cols)
			throw std::invalid_argument("Invalid matrix dimensions for assignment");

		std::memcpy(data, other.data, sizeof(float) * rows * cols);

		return *this;
	};
	void Mat2D::identity(Mat2D& m)
	{
		for (uint32_t i = 0; i < m.rows; i++)
			for (uint32_t j = 0; j < m.cols; j++)
				m[i][j] = i == j;
	};
	void Mat2D::print()
	{
		for (uint32_t i = 0; i < rows; i++)
		{
			for (uint32_t j = 0; j < cols; j++)
				std::cout << (*this)[i][j] << " ";
			std::cout << std::endl;
		};
	};
};
