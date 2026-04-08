#pragma once

#include "../Core/Core.h"
#include "Font.h"
#include "Shader.h"

namespace Mage {

    class MAGE_API TextRenderer : ICopyDisable
    {
        friend class Application ;

    public:
        ~TextRenderer() ;

		void render_text(Font& font, const char* text, float x, float y, float scale, const Color& color);

    private:
        explicit TextRenderer( Window& window ) ;
    
        struct Impl ;
        Impl* _impl ;

    } ;

}
