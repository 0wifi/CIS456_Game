#include "Color.h"

Mage::Color::Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255 )
    : r( r ), g( g ), b( b ), a( a )
{}

Mage::Color Mage::Color::custom( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
{
    return { r, g, b, a } ;
}

Mage::Color Mage::Color::custom( float r, float g, float b, float a )
{
    return {
        static_cast<uint8_t>( r * 255.0f ),
        static_cast<uint8_t>( g * 255.0f ),
        static_cast<uint8_t>( b * 255.0f ),
        static_cast<uint8_t>( a * 255.0f ) } ;

}

Mage::Color Mage::Color::pink            = Color( 255, 192, 203 ) ;
Mage::Color Mage::Color::crimson         = Color( 220,  20,  60 ) ;
Mage::Color Mage::Color::red             = Color( 255,   0,   0 ) ;
Mage::Color Mage::Color::maroon          = Color( 128,   0,   0 ) ;
Mage::Color Mage::Color::brown           = Color( 165,  42,  42 ) ;
Mage::Color Mage::Color::misty_rose      = Color( 255, 228, 225 ) ;
Mage::Color Mage::Color::salmon          = Color( 250, 128, 114 ) ;
Mage::Color Mage::Color::coral           = Color( 255, 127,  80 ) ;
Mage::Color Mage::Color::orange_red      = Color( 255,  69,   0 ) ;
Mage::Color Mage::Color::chocolate       = Color( 210, 105,  30 ) ;
Mage::Color Mage::Color::orange          = Color( 255, 165,   0 ) ;
Mage::Color Mage::Color::gold            = Color( 255, 215,   0 ) ;
Mage::Color Mage::Color::ivory           = Color( 255, 255, 240 ) ;
Mage::Color Mage::Color::yellow          = Color( 255, 255,   0 ) ;
Mage::Color Mage::Color::olive           = Color( 128, 128,   0 ) ;
Mage::Color Mage::Color::yellow_green    = Color( 154, 205,  50 ) ;
Mage::Color Mage::Color::lawn_green      = Color( 124, 252,   0 ) ;
Mage::Color Mage::Color::chartreuse      = Color( 127, 255,   0 ) ;
Mage::Color Mage::Color::lime            = Color(   0, 255,   0 ) ;
Mage::Color Mage::Color::green           = Color(   0, 128,   0 ) ;
Mage::Color Mage::Color::spring_green    = Color(   0, 255, 127 ) ;
Mage::Color Mage::Color::aquamarine      = Color( 127, 255, 212 ) ;
Mage::Color Mage::Color::turquoise       = Color(  64, 224, 208 ) ;
Mage::Color Mage::Color::azure           = Color( 240, 255, 255 ) ;
Mage::Color Mage::Color::cyan            = Color(   0, 255, 255 ) ;
Mage::Color Mage::Color::teal            = Color(   0, 128, 128 ) ;
Mage::Color Mage::Color::lavender        = Color( 230, 230, 250 ) ;
Mage::Color Mage::Color::blue            = Color(   0,   0, 255 ) ;
Mage::Color Mage::Color::navy            = Color(   0,   0, 128 ) ;
Mage::Color Mage::Color::blue_violet     = Color( 138,  43, 226 ) ;
Mage::Color Mage::Color::indigo          = Color(  75,   0, 130 ) ;
Mage::Color Mage::Color::dark_violet     = Color( 148,   0, 211 ) ;
Mage::Color Mage::Color::plum            = Color( 221, 160, 221 ) ;
Mage::Color Mage::Color::magenta         = Color( 255,   0, 255 ) ;
Mage::Color Mage::Color::purple          = Color( 128,   0, 128 ) ;
Mage::Color Mage::Color::red_violet      = Color( 199,  21, 133 ) ;
Mage::Color Mage::Color::tan             = Color( 210, 180, 140 ) ;
Mage::Color Mage::Color::beige           = Color( 245, 245, 220 ) ;
Mage::Color Mage::Color::slate_gray      = Color( 112, 128, 144 ) ;
Mage::Color Mage::Color::dark_slate_gray = Color(  47,  79,  79 ) ;
Mage::Color Mage::Color::white           = Color( 255, 255, 255 ) ;
Mage::Color Mage::Color::white_smoke     = Color( 245, 245, 245 ) ;
Mage::Color Mage::Color::light_gray      = Color( 211, 211, 211 ) ;
Mage::Color Mage::Color::silver          = Color( 192, 192, 192 ) ;
Mage::Color Mage::Color::dark_gray       = Color( 169, 169, 169 ) ;
Mage::Color Mage::Color::gray            = Color( 128, 128, 128 ) ;
Mage::Color Mage::Color::dim_gray        = Color( 105, 105, 105 ) ;
Mage::Color Mage::Color::black           = Color(   0,   0,   0 ) ;
