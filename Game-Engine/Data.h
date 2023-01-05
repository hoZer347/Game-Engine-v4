#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>

namespace loom
{
	typedef uint32_t ID;



	struct MeshInfo final
	{
		_NODISCARD static size_t count() noexcept { return _count; };

	protected:
		template <typename T>
		friend struct Attribute;
		static inline size_t _count = 0;

	private:
		MeshInfo() { };
	};



	template <typename T>
	struct Attribute final
	{
		Attribute(void(*kernel)(const T&) = nullptr)
		{
			if (claimed)
			{
				std::cerr << "This Attribute has already been claimed, consider using a struct containing what you want" << std::endl;
				exit(0);
			};

			claimed = true;

			Attribute<T>::kernel = kernel;
		};

		static inline bool claimed = false;
		static inline void(*kernel)(const T&) = nullptr;
		static inline const size_t count = MeshInfo::_count++;
	};



	struct Mesh final
	{
		Mesh()
		{
			meshes.push_back(this);
		};

		~Mesh()
		{
			for (auto i = 0; i < MeshInfo::count(); i++)
				delete data[i];

			delete[] data;
		};

		const void** data = (const void**)malloc(sizeof(void*) * MeshInfo::count());

		static inline std::vector<Mesh*> meshes;
	};



	struct Shader final	{ ID data; };
	struct Texture final { ID data; };

	struct Vtx final { vec4 data[4]; };
	struct Vtxs final { std::vector<Vtx> data; };
	struct Inds final { std::vector<uint32_t> data; };

	struct Draw final { ID data; };
};
