#include "TextRenderer.h"

namespace Mage {

    struct TextRenderer::Impl
    {
        const char* vs =
            R"( #version 330 core

                layout( location = 0 ) in vec2 pos ;
                layout( location = 1 ) in vec2 uv ;

                uniform vec2 window_size ;
                out vec2 texture_coords ;

                void main()
                {
                    mat4 ortho_projection = mat4( 1.0f ) ;
                    ortho_projection[ 0 ][ 0 ] = 2.0f / window_size.x ;
                    ortho_projection[ 1 ][ 1 ] = 2.0f / window_size.y ;
                    ortho_projection[ 2 ][ 2 ] = -1.0f ;
                    ortho_projection[ 3 ][ 0 ] = -1.0f ;
                    ortho_projection[ 3 ][ 1 ] = -1.0f ;
                    gl_Position = ortho_projection * vec4( pos, 0.0, 1.0 ) ;
                    texture_coords = uv ;
                } )" ;
        
        const char* fs =
            R"( #version 330 core

                in vec2 texture_coords ;
                out vec4 color ;
                
                uniform sampler2D text ;
                uniform vec4 text_color ;
                
                void main()
                {
                    vec4 sampled = vec4( 1.0, 1.0, 1.0, texture( text, texture_coords ).r ) ;
                    color = text_color * sampled ;
                } )" ;

        Shader  text_shader             = Shader( "TextShader", vs, fs ) ;
        Window* window                  =                        nullptr ;
        GLint   window_size_uniform_loc =                              0 ;
        GLint   text_color_uniform_loc  =                              0 ;
    } ;

    TextRenderer::TextRenderer( Window& window )
    {
        LOG_E_INFO( "TextRenderer starting." ) ;
        _impl = new Impl() ;
        _impl->window = &window ;
        _impl->window_size_uniform_loc = glGetUniformLocation( _impl->text_shader.get_shader_id(), "window_size" ) ;
        _impl->text_color_uniform_loc  = glGetUniformLocation( _impl->text_shader.get_shader_id(), "text_color"  ) ;
    }

    TextRenderer::~TextRenderer()
    {
        LOG_E_INFO( "TextRenderer stopping." ) ;
        delete _impl ;
    }

    void TextRenderer::render_text(Font& font, const char* text, float x, float y, float scale, const Color& color)
    {
		auto ww = static_cast<float>(_impl->window->get_width());
		auto wh = static_cast<float>(_impl->window->get_height());

		_impl->text_shader.use();

		glUniform2f(_impl->window_size_uniform_loc, ww, wh);
		glUniform4f(_impl->text_color_uniform_loc, color.r_float(), color.g_float(), color.b_float(), color.a_float());

        font.bind();

        //iterate through all characters
		for (size_t i = 0; i < std::strlen(text); ++i)
        {
            auto size_x = font.get_glyph_size_x( text[i] ) ;
            auto size_y = font.get_glyph_size_y( text[i] ) ;
			auto bearing_x = font.get_glyph_bearing_x(text[i]);
			auto bearing_y = font.get_glyph_bearing_y(text[i]);
            
            float x_pos = x + static_cast<float>(bearing_x) * scale;
            float y_pos = (wh - y) - static_cast<float>(size_y - bearing_y) * scale;

            float w = static_cast<float>(size_x) * scale;
            float h = static_cast<float>(size_y) * scale;

            font.draw_glyph(text[i], x_pos, y_pos, w, h);

            //see comment in font.measure_text
            x += static_cast<float>(font.get_glyph_advance(text[i]) >> 6) * scale;
        }
    }

}
