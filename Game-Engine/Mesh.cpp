#include "Mesh.h"

#include "Data.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

namespace loom
{
	struct MeshManager final :
		virtual protected Loadable,
		virtual protected Updateable,
		virtual protected Renderable
	{
		void load() override
		{ };
		void update() override
		{ };
		void render() override
		{ };
	};
	static inline MeshManager manager;


	Mesh::Mesh(Vtxs&& _vtxs, Inds&& _inds)
	{

	};
	Mesh::~Mesh()
	{
		inds.access([](Inds& _inds)
		{
			std::shared_ptr<Inds> __inds = unused_inds.acquire();

			for (auto& ind : _inds)
				__inds->push_back(ind);

			unused_inds.release(__inds);
		});
	};
};
