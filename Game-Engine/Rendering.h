#pragma once

#include "Task.h"
#include "Data.h"

namespace loom
{
	template <typename T> struct Buffer;

	// TODO: Load Textures / Shaders from a file guide

	void create_shader(Shader& dst, std::string file_name);
	void create_shader(Shader& dst, std::vector<std::string> file_names);
	void create_texture(Texture& dst, std::string file_name, Texture_Type type);

	//_NODISCARD Task _create_shader(Shader& dst, std::string file_name) noexcept;
	//_NODISCARD Task _create_shader(Shader& dst, std::vector<std::string> file_names) noexcept;
	//_NODISCARD Task _create_texture(Texture& dst, std::string file_name, Texture_Type type) noexcept;
	
	_NODISCARD Task _render(Buffer<Mesh>& buffer) noexcept;
};
