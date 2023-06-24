#pragma once

#include "Data.h"

namespace loom
{
	typedef bool(*AnimateTask)(mat4&);

	struct Animation final :
		virtual public Updateable
	{
		Animation(mat4& trns) :
			trns(trns)
		{ };
		void update() override
		{
			if (!animations.empty() && animations.back()(trns))
				animations.pop_back();
		};
		void assign(AnimateTask&& task)
		{
			assert((Engine::GetIsOnUpdater(), "Animation accessed outside updater"));
			animations.emplace_back(task);
		};

	private:
		mat4& trns;
		std::vector<AnimateTask> animations;
	};
};
