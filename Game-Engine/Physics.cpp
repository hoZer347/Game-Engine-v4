#include "Physics.h"

namespace loom
{
	struct CollisionManager :
		virtual protected Physicsable
	{
		void physics() override
		{
			for (auto& group : groups)
				group->do_collision();
		};

		static inline std::vector<CollisionGroup*> groups;
	};
	static inline CollisionManager manager;
	
	
	CollisionGroup::CollisionGroup(std::vector<_Geometry*>&& objs) :
		objs(objs)
	{
		CollisionManager::groups.emplace_back(this);
	};
	void CollisionGroup::push(std::vector<_Geometry*>&& objs)
	{
		this->objs.insert(this->objs.end(), objs.begin(), objs.end());
	};
	void CollisionGroup::do_collision()
	{
		// TODO: Make this more efficient O(n^2) -> O(nlogn)
		for (auto i = 0; i < objs.size(); i++)
			for (auto j = i + 1; j < objs.size(); j++)
				collide(objs[i], objs[j]);
	}
	void CollisionGroup::collide(_Geometry* g0, _Geometry* g1)
	{
		switch (g0->collision_type || g1->collision_type)
		{


		default:
			break;
		};
	};
};
