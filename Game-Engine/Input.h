#pragma once

#include "Data.h"

namespace loom
{
	struct Input : public Updatable
	{
	private:
		void load() override;
		void update() override;
		void unload() override;
	};
};
