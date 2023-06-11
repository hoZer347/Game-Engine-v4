#include "Cuda.cuh"

#include "Matrix.h"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include <stdio.h>
#include <iostream>

#include <chrono>
#include <thread>

namespace loom
{
	__global__
	void matrixMultiplyKernel(const float* m0, const float* m1, float* m2, const uint32_t rows, const uint32_t cols, const uint32_t commonDim)
	{
		// Calculate the row and column indices for the current thread
		uint32_t row = blockIdx.y * blockDim.y + threadIdx.y;
		uint32_t col = blockIdx.x * blockDim.x + threadIdx.x;

		// Perform matrix multiplication for the current element
		if (row < rows && col < cols)
		{
			float value = 0.0f;
			for (uint32_t k = 0; k < commonDim; ++k)
			{
				value += m0[row * commonDim + k] * m1[k * cols + col];
			}
			m2[row * cols + col] = value;
		}
	};



	void Mat2D::multiply(Mat2D& m0, Mat2D& m1, Mat2D& m2)
	{
		// Check if matrix dimensions are compatible for multiplication
		if (m0.cols != m1.rows || m0.rows != m2.rows || m1.cols != m2.cols)
			throw std::invalid_argument("Incompatible matrix dimensions for multiplication");

		// Allocate GPU memory for matrices
		float* d_m0, * d_m1, * d_m2;
		size_t sizeM0 = m0.rows * m0.cols * sizeof(float);
		size_t sizeM1 = m1.rows * m1.cols * sizeof(float);
		size_t sizeM2 = m2.rows * m2.cols * sizeof(float);
		cudaMalloc((void**)&d_m0, sizeM0);
		cudaMalloc((void**)&d_m1, sizeM1);
		cudaMalloc((void**)&d_m2, sizeM2);

		// Copy matrix data from host to GPU memory
		cudaMemcpy(d_m0, m0.data, sizeM0, cudaMemcpyHostToDevice);
		cudaMemcpy(d_m1, m1.data, sizeM1, cudaMemcpyHostToDevice);

		// Set the CUDA kernel launch configuration
		dim3 blockSize(16, 16);  // 16x16 thread block
		dim3 gridSize((m2.cols + blockSize.x - 1) / blockSize.x, (m2.rows + blockSize.y - 1) / blockSize.y);  // grid size based on output matrix dimensions

		// Launch the matrix multiplication kernel on the GPU
		matrixMultiplyKernel<<<gridSize, blockSize>>>(d_m0, d_m1, d_m2, m2.rows, m2.cols, m0.cols);

		// Copy the result matrix from GPU to host memory
		cudaMemcpy(m2.data, d_m2, sizeM2, cudaMemcpyDeviceToHost);

		// Free GPU memory
		cudaFree(d_m0);
		cudaFree(d_m1);
		cudaFree(d_m2);
	};
};
