#pragma once

#include <cstdint>

namespace loom
{
	struct Mat2D final
	{
		typedef void(*_gen_func)(float*, const uint32_t&, const uint32_t&);

		Mat2D(uint32_t rows, uint32_t cols, _gen_func = nullptr);
		~Mat2D();

		static void multiply(Mat2D& m0, Mat2D& m1, Mat2D& m2);
		static void identity(Mat2D& m);
		
		void transpose(Mat2D& result);

		void print();

		template <typename T>
		float* operator[](T i) { return &data[cols * i]; }

		Mat2D& operator*(Mat2D& mat);
		Mat2D& operator=(const Mat2D& other);

		const uint32_t rows, cols;

		constexpr static void _gen_identity(
			float* data,
			const uint32_t& rows,
			const uint32_t& cols)
		{
			for (uint32_t i = 0; i < rows; i++)
				for (uint32_t j = 0; j < cols; j++)
					data[i * rows + j] = i == j;
		};

		constexpr static void _gen_value_is_index(
			float* data,
			const uint32_t& rows,
			const uint32_t& cols)
		{
			for (uint32_t i = 0; i < rows * cols; i++)
				data[i] = (float)i;
		};

	private:
		float* data;
	};
};
