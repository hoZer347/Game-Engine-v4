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
		virtual void load(std::shared_ptr<Window> window)=0;

		struct alignas(64) Vtx
		{
			vec4 data[4];
		};

		struct r_Obj
		{
			std::vector<Vtx>			vtxs;
			std::vector<unsigned int>	inds;
		};

		std::vector<unsigned int>	texs;

		unsigned int
			shader = 0,
			draw_mode = 0x0007;

		std::shared_ptr<Window> window;
	};

	struct Mesh : public Renderable
	{
		void load(std::shared_ptr<Window> window);

	protected:
		friend struct Data<Mesh>;
		Mesh();
	};

	struct Sprite : public Renderable
	{
		void load(std::shared_ptr<Window> window);

	protected:
		friend struct Data<Sprite>;
		Sprite();
	};
};
