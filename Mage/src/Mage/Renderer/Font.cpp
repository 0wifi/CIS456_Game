#include "Font.h"

namespace Mage {
	struct Font::Impl
	{
		struct Character
		{
			GLuint texture_id;
			glm::ivec2 size;
			glm::ivec2 bearing;
			unsigned int advance;
		};

		std::map<unsigned char, Character> characters;

		std::string font_file;
		uint_fast16_t font_size;
		uint_fast32_t line_height;
		GLuint vao = 0;
		GLuint vbo = 0;
	};

	Font::Font(const char* font_file, uint_fast16_t font_size)
	{
		LOG_E_INFO("Loading font: '%s' (%d)", font_file, font_size);

		_impl = new Impl();
		_impl->font_file = font_file;
		_impl->font_size = font_size;

		FT_Error error;

		FT_Library ft;
		if (error = FT_Init_FreeType(&ft))
		{
			std::stringstream ss;
			ss << "Failed initializing FreeType (" << error << ")";
			throw Exception(ss.str().c_str());
		}

		FT_Face face;
		if (error = FT_New_Face(ft, _impl->font_file.c_str(), 0, &face))
		{
			std::stringstream ss;
			ss << "Failed loading font: '" << _impl->font_file.c_str() << "' (" << error << ")";
			throw Exception(ss.str().c_str());
		}

		FT_Set_Pixel_Sizes(face, 0, _impl->font_size);

		if (error = FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		{
			std::stringstream ss;
			ss << "Failed loading 'X' glyph from font (needed to determine line height) (" << error << ")";
			throw Exception(ss.str().c_str());
		}

		_impl->line_height = face->glyph->bitmap.rows;

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (unsigned char c = 0; c < 128; ++c)
		{
			if (error = FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::stringstream ss;
				ss << "Failed loading '" << c << "' glyph from font (" << error << ")";
				LOG_E_WARN(ss.str().c_str());
			}

			//pick up from here
		}
	}
}
