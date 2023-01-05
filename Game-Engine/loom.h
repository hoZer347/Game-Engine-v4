#pragma once

#include "Data.h"

#include <thread>
#include <string>
#include <vector>
#include <barrier>
#include <functional>
#include <unordered_set>

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	void Init();
	void Render();
	void Exit();

	template <typename T>
	_NODISCARD static T& Make(Mesh& mesh, auto... args)
	{ return *(T*)(mesh.data[Attribute<T>::count] = new T(args...)); };

	template<typename T>
	_NODISCARD static T& Get(const Mesh& mesh)
	{ return *(T*)mesh.data[Attribute<T>::count]; };

	template <typename T>
	_NODISCARD static const T& View(const Mesh& mesh)
	{ return *(T*)mesh.data[Attribute<T>::count]; };
};
