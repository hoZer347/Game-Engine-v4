#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include <mutex>
#include <queue>
#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <functional>

#ifdef STOP_STRUCT_PRIVACY
#define protected public
#define private public
#endif

namespace loom
{
	// Basic types
	typedef uint32_t ID;
	typedef uint32_t TYPE;
	typedef uint32_t Ind;
	//



	// RenderObj
	struct RenderObj
	{
		virtual ~RenderObj();

	protected:
		friend struct Window;
		RenderObj() { };
	};
	//



	// Shader
	struct Shader final : public RenderObj
	{
		Shader(std::string files...)
		: files({ files })
		{ };

	protected:
		friend struct Mesh;
		const std::vector<std::string> files;
		ID id = -1;
	};
	//



	// Textures
	struct Textures final : public RenderObj
	{
		Textures(std::string files...)
		: files({ files })
		{ };

		// TODO: Make work with other RGB types
		// TODO: Add different wrapping options

	protected:
		friend struct Mesh;
		std::vector<std::string> files;
		std::vector<ID> ids;
	};
	//



	// Draw
	struct Draw final : public RenderObj
	{
		Draw(TYPE type)
		: type(type)
		{ };

	protected:
		friend struct Mesh;
		const TYPE type;
	};
	//



	// Buffer
	struct _Buffer : public RenderObj
	{
		// TODO: Add more attribute types
		// TODO: Add functionality for GL_BUFFER types
		void AddAttribute(size_t size, std::string name);

	protected:
		_Buffer(const TYPE array_type, const TYPE render_type, const size_t size, const void* ptr)
		: array_type(array_type), render_type(render_type), size(size), ptr(ptr)
		{ };

		friend struct Mesh;
		friend struct Window;
		const TYPE array_type;
		const TYPE render_type;
		ID id = 0;
		size_t size = 0;
		const void* ptr = nullptr;

		std::vector<std::pair<size_t, std::string>> attributes;
	};
	template <size_t NUM_VALUES, typename T=float>
	struct Buffer final : public _Buffer
	{
		Buffer(const TYPE array_type, const TYPE render_type, std::vector<T> data = { })
		: _Buffer(array_type, render_type, NUM_VALUES * sizeof(T), data.data())
		{ };

		void operator=(std::vector<T> new_data) { data = new_data; size = data.size() * sizeof(T); ptr = data.data(); }

	private:
		std::vector<T> data;
	};
	//



	// Mesh
	struct Mesh final : public RenderObj
	{
		Mesh(Shader& shader, Textures& textures, Draw& draw, _Buffer& buffers...)
		: shader(shader), textures(textures), draw(draw), buffers({ &buffers })
		{ };

		~Mesh();

		void load();

	protected:
		friend struct Window;

		Shader& shader;
		Textures& textures;
		Draw& draw;

		std::vector<_Buffer*> buffers;
	};
	//
};
