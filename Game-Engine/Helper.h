#pragma once

#include "Data.h"

namespace loom
{
	struct Helper final : public Object
	{
		Helper();
		static void assign(Task task);

	private:
		void load() override;
		void unload() override;
	};


	struct SyncHelper final : public Object
	{
		SyncHelper();
		static void assign(Task task);

	private:
		void load() override;
		void unload() override;
	};
};
