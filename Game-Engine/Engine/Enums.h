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
		VEC4_0_16,

		VEC4_0_32,
		VEC4_1_32,

		VEC4_0_64,
		VEC4_1_64,
		VEC4_2_64,
		VEC4_3_64,
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
