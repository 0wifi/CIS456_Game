#include "SpriteRenderer.h"

namespace Mage {

    struct SpriteRenderer::Impl
    {
        const char* vs =
            R"( #version 330 core
                
                layout( location = 0 ) in vec2 pos ;
                layout( location = 1 ) in vec2 uv  ;

                out vec2 texture_coords ;

                uniform vec4  camera      ;
                uniform vec2  sprite_size ;
                uniform vec2  translation ;
                uniform vec2  scale       ;
                uniform float rotation    ;

                void main()
                {
					mat4 ortho_projection = mat4( 1.0f ) ;
					ortho_projection[ 0 ][ 0 ] = 2.0f / ( camera.y - camera.x ) ;
					ortho_projection[ 1 ][ 1 ] = 2.0f / ( camera.w - camera.z ) ;
					ortho_projection[ 2 ][ 2 ] = -1.0f ;
					ortho_projection[ 3 ][ 0 ] = -1.0f * ( camera.y + camera.x ) / ( camera.y - camera.x ) ;
					ortho_projection[ 3 ][ 1 ] = -1.0f * ( camera.w + camera.z ) / ( camera.w - camera.z ) ;
		
                    mat4 translate_and_translate_half_scale = mat4( 1.0f ) ;
                    translate_and_translate_half_scale[ 3 ][ 0 ] = translation.x + 0.5f * scale.x * sprite_size.x ;
                    translate_and_translate_half_scale[ 3 ][ 1 ] = translation.y + 0.5f * scale.y * sprite_size.y ;

					float rotation_radians = radians( rotation ) ;
					
                    mat4 transform_rotation = mat4( 1.0f ) ;
					transform_rotation[ 0 ][ 0 ] = cos( rotation_radians ) ;
					transform_rotation[ 0 ][ 1 ] = -sin( rotation_radians ) ;
					transform_rotation[ 1 ][ 0 ] = sin( rotation_radians ) ;
					transform_rotation[ 1 ][ 1 ] = cos( rotation_radians ) ;
					
                    mat4 translate_half_scale_back = mat4( 1.0f ) ;
		            translate_half_scale_back[ 3 ][ 0 ] = -0.5f * scale.x * sprite_size.x ;
		            translate_half_scale_back[ 3 ][ 1 ] = -0.5f * scale.y * sprite_size.y ;

                    mat4 transform_scale = mat4( 1.0f ) ;
                    transform_scale[ 0 ] *= scale.x * sprite_size.x ;
                    transform_scale[ 1 ] *= scale.y * sprite_size.y ;
					
                    mat4 transform = translate_and_translate_half_scale ;
					transform *= transform_rotation ;
					transform *= translate_half_scale_back ;
                    transform *= transform_scale ;

                    gl_Position = ortho_projection * transform * vec4( pos, 0.0, 1.0 ) ;
                    texture_coords = uv ;
                } )" ;

        const char* fs =
            R"( #version 330 core

                in vec2 texture_coords ;
                
                out vec4 color ;

                uniform sampler2D sprite      ;
                uniform vec4      in_color    ;
                uniform int       use_texture ;

                void main()
                {
                    if( use_texture == 1 )
                        color = texture( sprite, texture_coords ) ;
                    else
                        color = in_color ;
                } )" ;

        Shader  shader                  = Shader( "Shader", vs, fs ) ;
        Camera* camera                  = nullptr ;

        GLint   camera_uniform_loc      = 0 ;
        GLint   sprite_size_uniform_loc = 0 ;
        GLint   translation_uniform_loc = 0 ;
        GLint   scale_uniform_loc       = 0 ;
        GLint   rotation_uniform_loc    = 0 ;
        GLint   color_uniform_loc       = 0 ;
        GLint   use_texture_uniform_loc = 0 ;
    } ;

    SpriteRenderer::SpriteRenderer( Camera& camera )
    {
        LOG_E_INFO( "SpriteRenderer starting." ) ;
        _impl = new Impl() ;
        _impl->camera = &camera ;

        _impl->camera_uniform_loc      = glGetUniformLocation( _impl->shader.get_shader_id(), "camera"      ) ;
        _impl->sprite_size_uniform_loc = glGetUniformLocation( _impl->shader.get_shader_id(), "sprite_size" ) ;
        _impl->translation_uniform_loc = glGetUniformLocation( _impl->shader.get_shader_id(), "translation" ) ;
        _impl->scale_uniform_loc       = glGetUniformLocation( _impl->shader.get_shader_id(), "scale"       ) ;
        _impl->rotation_uniform_loc    = glGetUniformLocation( _impl->shader.get_shader_id(), "rotation"    ) ;
        _impl->color_uniform_loc       = glGetUniformLocation( _impl->shader.get_shader_id(), "in_color"    ) ;
        _impl->use_texture_uniform_loc = glGetUniformLocation( _impl->shader.get_shader_id(), "use_texture" ) ;
    }

    SpriteRenderer::~SpriteRenderer()
    {
        LOG_E_INFO( "SpriteRenderer stopping." ) ;
        delete _impl ;
    }

    void SpriteRenderer::render( const Sprite& sprite,
        const glm::vec2& translation, const glm::vec2& scale, float rotation, float elapsed_time ) const
    {
        _impl->shader.use() ;

        glUniform4f( _impl->camera_uniform_loc, _impl->camera->left,
            _impl->camera->right, _impl->camera->bottom, _impl->camera->top ) ;

        glUniform2f( _impl->sprite_size_uniform_loc,
            static_cast<float>( sprite.get_width() ),
            static_cast<float>( sprite.get_height() ) ) ;

        glUniform2f( _impl->translation_uniform_loc, translation.x, translation.y ) ;
        glUniform2f( _impl->scale_uniform_loc, scale.x, scale.y ) ;
        glUniform1f( _impl->rotation_uniform_loc, rotation ) ;

        glUniform4f( _impl->color_uniform_loc,
            sprite.get_color().r_float(), sprite.get_color().g_float(),
            sprite.get_color().b_float(), sprite.get_color().a_float() ) ;

        // Tell shader whether to sample texture or use solid color
        glUniform1i( _impl->use_texture_uniform_loc, sprite.is_textured() ? 1 : 0 ) ;

        sprite.draw( elapsed_time ) ;
    }

}
