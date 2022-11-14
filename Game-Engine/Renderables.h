#pragma once

#include "Data.h"

#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
using namespace glm;

#include <vector>

namespace eng
{
	struct Window;

	struct TextureManager;
	struct ShaderManager;

	struct Renderable
	{
		void load_shader(std::vector<std::string> file_names);
		void load_textures(std::vector<std::pair<std::string, unsigned int>> file_names);
		static void load(std::shared_ptr<Window> window);
		virtual void render()=0;

		struct alignas(64) Vtx { vec4 data[4]; };

		std::vector<Vtx>			vtxs;
		std::vector<unsigned int>	inds;
		std::vector<unsigned int>	texs;

		unsigned int
			shader = 0,
			draw_mode = 0x0007;

	protected:
		static inline std::atomic<bool> should_draw = true;
		static inline std::shared_ptr<Window> window;
	};

	struct Mesh : public Renderable
	{
		void render() override;

		static void test();

	protected:
		friend struct Data<Mesh>;
		friend struct Buffer<Mesh>;
		Mesh() { };
	};

	struct Sprite : public Renderable
	{
	protected:
		friend struct Data<Mesh>;
		friend struct Buffer<Mesh>;
		Sprite() { };
	};
};
