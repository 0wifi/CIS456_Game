#include "Font.h"

namespace Mage {

    struct Font::Impl
    {
        struct Character
        {
            GLuint       texture_id ;
            glm::ivec2   size       ;
            glm::ivec2   bearing    ;
            unsigned int advance    ;
        } ;

        std::map<unsigned char, Character> characters ;

        std::string   font_file   ;
        uint_fast16_t font_size   ;
        uint_fast32_t line_height ;
        GLuint        vao     = 0 ;
        GLuint        vbo     = 0 ;
    } ;

    Font::Font( const char* font_file, uint_fast16_t font_size )
    {
        LOG_E_INFO( "Loading font: '%s' (%d)", font_file, font_size ) ;

        _impl = new Impl() ;
        _impl->font_file = font_file ;
        _impl->font_size = font_size ;

        FT_Error error ;
        
        FT_Library ft ;
        if( ( error = FT_Init_FreeType( &ft ) ) )
        {
            std::stringstream ss ;
            ss << "Failed initializing FreeType (" << error << ")" ;
            throw Exception( ss.str().c_str() ) ;
        }

        FT_Face face ;
        if( ( error = FT_New_Face( ft, _impl->font_file.c_str(), 0, &face ) ) )
        {
            std::stringstream ss ;
            ss << "Failed loading font: " << _impl->font_file << " (" << error << ")" ;
            throw Exception( ss.str().c_str() ) ;
        }

        FT_Set_Pixel_Sizes( face, 0, _impl->font_size ) ;

        if( ( error = FT_Load_Char( face, 'X', FT_LOAD_RENDER ) ) )
        {
            std::stringstream ss ;
            ss << "Failed loading 'X' glyph from font (needed to determine line height) ("
               << error << ")" ;
            throw Exception( ss.str().c_str() ) ;
        }

        _impl->line_height = face->glyph->bitmap.rows ;

        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ) ;

        for( unsigned char c = 0 ; c < 128 ; ++c )
        {
            if( ( error = FT_Load_Char( face, c, FT_LOAD_RENDER ) ) )
            {
                std::stringstream ss ;
                ss << "Failed loading '" << c << "' glyph from font (" << error << ")" ;
                LOG_E_WARN( ss.str().c_str() ) ;
                continue ;
            }

            GLuint texture ;
            glGenTextures( 1, &texture ) ;
            glBindTexture( GL_TEXTURE_2D, texture ) ;

            glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RED,
                static_cast<GLsizei>(face->glyph->bitmap.width),
                static_cast<GLsizei>(face->glyph->bitmap.rows),
                0, GL_RED, GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
			);
            
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) ;
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) ;
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) ;
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) ;

            _impl->characters.insert(std::pair<unsigned char, Impl::Character>(
                c,
                {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x) 
                }
			));
        }

        glBindTexture(GL_TEXTURE_2D, 0);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        auto first_pos_byte = reinterpret_cast<void*>( 0 ) ;
        auto first_uv_byte  = reinterpret_cast<void*>( sizeof( GLfloat ) * 2 ) ;

        glGenVertexArrays( 1, &_impl->vao ) ;
        glGenBuffers( 1, &_impl->vbo ) ;
        glBindVertexArray( _impl->vao ) ;
        glBindBuffer( GL_ARRAY_BUFFER, _impl->vbo ) ;
        glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 6 * 4, nullptr, GL_DYNAMIC_DRAW ) ;
        glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), first_pos_byte ) ;
        glEnableVertexAttribArray( 0 ) ;
        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), first_uv_byte ) ;
        glEnableVertexAttribArray( 1 ) ;
        glBindBuffer( GL_ARRAY_BUFFER, 0 ) ;
        glBindVertexArray( 0 ) ;
    }

    Font::~Font()
    {
        LOG_E_INFO( "Unloading font: '%s'", _impl->font_file.c_str() ) ;
        for( auto& p : _impl->characters )
            glDeleteTextures( 1, &p.second.texture_id ) ;
        glDeleteBuffers( 1, &_impl->vbo ) ;
        glDeleteBuffers( 1, &_impl->vao ) ;
        delete _impl ;
    }
    
    uint_fast32_t Font::get_line_height() const
    {
        return _impl->line_height ;
    }
    
    uint_fast32_t Font::get_glyph_size_x( unsigned char glyph ) const
    {
        return _impl->characters[ glyph ].size.x ;
    }

    uint_fast32_t Font::get_glyph_size_y( unsigned char glyph ) const
    {
        return _impl->characters[ glyph ].size.y ;
    }

    int_fast32_t Font::get_glyph_bearing_x( unsigned char glyph ) const
    {
        return _impl->characters[ glyph ].bearing.x ;
    }

    int_fast32_t Font::get_glyph_bearing_y( unsigned char glyph ) const
    {
        return _impl->characters[ glyph ].bearing.y ;
    }
    
    uint32_t Font::get_glyph_advance( unsigned char glyph ) const
    {
        return _impl->characters[ glyph ].advance ;
    }

    float Font::measure_text( const char* text, float scale ) const
    {
        float x = 0.0f ;
        for (size_t i = 0; i < std::strlen(text); ++i)
        {
			//advance to next glyph (note the advance is number of 1/64 pixels)
			// bit shift by 6 to get value in pixels (2^6 = 64)
			x += static_cast<float>(get_glyph_advance(text[i]) >> 6) * scale;
        }
        return x;
	}

    void Font::bind()
    {
		glActiveTexture( GL_TEXTURE0 );
		glBindVertexArray( _impl->vao );
    }

    void Font::draw_glyph( unsigned char glyph, float x, float y, float width, float height )
    {
        if (glyph > 127)
        {
			LOG_E_WARN("Attempting to draw unsupported glyph: %d", glyph);
            return;
        }
        float vertices[ 6 ][ 4 ] = {
			//pos                      // uv
            { x,         y + height,   0.0f, 0.0f },
            { x,         y,            0.0f, 1.0f },
            { x + width, y,            1.0f, 1.0f },
            { x,         y + height,   0.0f, 0.0f },
            { x + width, y,            1.0f, 1.0f },
            { x + width, y + height,   1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, _impl->characters[glyph].texture_id);
		glBindBuffer(GL_ARRAY_BUFFER, _impl->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void Font::unbind()
    {
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}
