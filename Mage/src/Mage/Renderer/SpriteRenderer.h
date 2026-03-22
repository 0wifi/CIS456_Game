#pragma once

#include "../Core/Core.h"
#include "Camera.h"
#include "Shader.h"
#include "Sprite.h"

namespace Mage {

    class MAGE_API SpriteRenderer : ICopyDisable  // NOLINT(cppcoreguidelines-special-member-functions)
    {
        friend class Application ;

    public:
        ~SpriteRenderer() ;

        void render( const Sprite& sprite, const glm::vec2& translation, const glm::vec2& scale, float rotation, float elapsed_time ) const ;

    private:
        explicit SpriteRenderer( Camera& camera ) ;

        struct Impl ;
        Impl* _impl ;

    } ;

}