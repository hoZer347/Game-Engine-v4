#include "Engine.h"

// OpenGL
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;
//

#include <mutex>

#include "ft2build.h"
#include FT_FREETYPE_H

namespace eng
{
	static inline FT_Library library;
	static inline FT_Long index;

	void Text::_init()
	{
		FT_Init_FreeType(&library);
	};

    void Text::load_font(Material& mat, const char* file_name)
	{
		FT_Face face;
		FT_New_Face(library, file_name, index++, &face);
		
		FT_Set_Pixel_Sizes(face, 0, 48);

        for (unsigned char c = 67; c <68; c++)
        {
            // load character glyph 
            FT_Load_Char(face, c, FT_LOAD_RENDER);
            
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            // now store character for later use
            //Character character = {
            //    texture,
            //    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            //    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            //    face->glyph->advance.x
            //};
            //Characters.insert(std::pair<char, Character>(c, character));

            mat.texs.push_back(texture);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        FT_Done_Face(face);
	};
};
