#pragma once

#include "Core.h"

// Found the list of 48 named colors at:
//   https://simple.wikipedia.org/wiki/Template:Web_colors

namespace Mage {

    class MAGE_API Color final
    {
    public:
        uint8_t r ;
        uint8_t g ;
        uint8_t b ;
        uint8_t a ;

        float r_float() const { return static_cast<float>( r ) / 255.0f ; }
        float g_float() const { return static_cast<float>( g ) / 255.0f ; }
        float b_float() const { return static_cast<float>( b ) / 255.0f ; }
        float a_float() const { return static_cast<float>( a ) / 255.0f ; }

        static Color pink            ;
        static Color crimson         ;
        static Color red             ;
        static Color maroon          ;
        static Color brown           ;
        static Color misty_rose      ;
        static Color salmon          ;
        static Color coral           ;
        static Color orange_red      ;
        static Color chocolate       ;
        static Color orange          ;
        static Color gold            ;
        static Color ivory           ;
        static Color yellow          ;
        static Color olive           ;
        static Color yellow_green    ;
        static Color lawn_green      ;
        static Color chartreuse      ;
        static Color lime            ;
        static Color green           ;
        static Color spring_green    ;
        static Color aquamarine      ;
        static Color turquoise       ;
        static Color azure           ;
        static Color cyan            ;
        static Color teal            ;
        static Color lavender        ;
        static Color blue            ;
        static Color navy            ;
        static Color blue_violet     ;
        static Color indigo          ;
        static Color dark_violet     ;
        static Color plum            ;
        static Color magenta         ;
        static Color purple          ;
        static Color red_violet      ;
        static Color tan             ;
        static Color beige           ;
        static Color slate_gray      ;
        static Color dark_slate_gray ;
        static Color white           ;
        static Color white_smoke     ;
        static Color light_gray      ;
        static Color silver          ;
        static Color dark_gray       ;
        static Color gray            ;
        static Color dim_gray        ;
        static Color black           ;

        static Color custom( uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255 ) ;

        static Color custom( float r, float g, float b, float a = 1.0f ) ;

    private:
        Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) ;

    } ;

}
