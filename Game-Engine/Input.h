#pragma once

#include "Data.h"

namespace loom
{
	struct Input : public Updatable
	{
	private:
		void update() override;
	};
};
