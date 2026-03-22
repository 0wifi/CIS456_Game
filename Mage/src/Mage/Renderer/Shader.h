#pragma once

#include "../Core/Core.h"

namespace Mage {

    class MAGE_API Shader : ICopyDisable  // NOLINT(cppcoreguidelines-special-member-functions)
    {
    public:
        Shader( const char* shader_name, const char* vertex_shader_src, const char* fragment_shader_src ) ;

        virtual ~Shader() ;

        uint32_t get_shader_id() const ;

        void use() const ;

    private:
        struct Impl ;
        Impl* _impl ;

    } ;

}
