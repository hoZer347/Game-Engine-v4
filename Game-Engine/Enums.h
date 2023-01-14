#pragma once

namespace loom
{
	enum // Vertex Sizes
	{
		V_16,
		V_32,
		V_64,
	};

	enum // Vertex Attributes
	{
		VEC4_0_16, // 0

		VEC4_0_32, // 1
		VEC4_1_32, // 2

		VEC4_0_64, // 3
		VEC4_1_64, // 4
		VEC4_2_64, // 5
		VEC4_3_64, // 6
	};

	enum // Useful Matrices
	{
		MVP,
		MODE,
		PROJ,
		VIEW,
		
		TRNS,
		ROTN,

		ROLL,
		PTCH,
		YAWW,
	};
};
