#include "Shader.h"

namespace Mage {

    struct Shader::Impl
    {
        uint32_t    shader_id = 0 ;
        std::string shader_name   ;

        void compile( const char* vertex_code, const char* fragment_code )
        {
            shader_id = glCreateProgram() ;

            if( ! shader_id )
                throw Exception( ( "Failed creating shader program: " + std::string( shader_name ) ).c_str() ) ;

            add( vertex_code, GL_VERTEX_SHADER ) ;
            add( fragment_code, GL_FRAGMENT_SHADER ) ;

            GLint result = 0;
            GLchar error_log[ 1024 ] = { 0 } ;

            glLinkProgram( shader_id );
            glGetProgramiv( shader_id, GL_LINK_STATUS, &result ) ;
            if( ! result )
            {
                glGetProgramInfoLog( shader_id, sizeof( error_log ), nullptr, error_log ) ;
                throw Exception( ( "Error linking shader program: " + std::string( error_log ) ).c_str() ) ;
            }

            glValidateProgram( shader_id ) ;
            glGetProgramiv( shader_id, GL_VALIDATE_STATUS, &result ) ;
            if( ! result )
            {
                glGetProgramInfoLog( shader_id, sizeof( error_log ), nullptr, error_log ) ;
                throw Exception( ( "Error validating shader program: " + std::string( error_log ) ).c_str() ) ;
            }
        }

        void add( const char* shader_code, const GLenum shader_type ) const
        {
            const GLuint the_shader = glCreateShader( shader_type ) ;

            const GLchar* the_code[ 1 ] ;
            the_code[ 0 ] = shader_code ;

            GLint code_length[ 1 ] ;
            code_length[ 0 ] = static_cast<GLint>( std::strlen( shader_code ) ) ;

            glShaderSource( the_shader, 1, the_code, code_length ) ;
            glCompileShader( the_shader ) ;

            GLint result = 0 ;
            GLchar error_log[ 1024 ] = { 0 } ;

            glGetShaderiv( the_shader, GL_COMPILE_STATUS, &result ) ;
            if( !result )
            {
                glGetShaderInfoLog( the_shader, 1024, nullptr, error_log ) ;
                std::stringstream message ;
                message << "Error compiling shader (shader type " << shader_type << "): " << error_log ;
                throw Exception( message.str().c_str() ) ;
            }

            glAttachShader( shader_id, the_shader ) ;
        }
    } ;

    Shader::Shader( const char* shader_name, const char* vertex_shader_src, const char* fragment_shader_src )
    {
        _impl = new Impl() ;
        _impl->shader_name = shader_name ;
        _impl->compile( vertex_shader_src, fragment_shader_src ) ;
        LOG_E_INFO( "Shader created: '%s'", shader_name ) ;
    }

    Shader::~Shader()
    {
        if( _impl->shader_id != 0 )
            glDeleteProgram( _impl->shader_id ) ;
        LOG_E_INFO( "Shader destroyed: '%s'", _impl->shader_name.c_str() ) ;
        delete _impl ;
    }

    uint32_t Shader::get_shader_id() const
    {
        return _impl->shader_id ;
    }

    void Shader::use() const
    {
        glUseProgram( _impl->shader_id ) ;
    }

}
