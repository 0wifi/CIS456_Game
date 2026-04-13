#pragma once

#include "../Core/Core.h"

namespace Mage {

    class MAGE_API Font : ICopyDisable
    {
    public:
        Font( const char* font_file, uint_fast16_t font_size ) ;

        ~Font() ;

        uint_fast32_t get_line_height() const ;

        uint_fast32_t get_glyph_size_x( unsigned char glyph ) const ;
        uint_fast32_t get_glyph_size_y( unsigned char glyph ) const ;

        int_fast32_t get_glyph_bearing_x( unsigned char glyph ) const ;
        int_fast32_t get_glyph_bearing_y( unsigned char glyph ) const ;

        uint_fast32_t get_glyph_advance( unsigned char glyph ) const ;

        float measure_text( const char* text, float scale ) const ;

        void bind() ;

        void unbind() ;

        void draw_glyph( unsigned char glyph, float x, float y, float width, float height ) ;

    private:
        struct Impl ;
        Impl* _impl ;

    } ;
}
