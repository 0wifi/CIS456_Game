#pragma once

#include "Core.h"

namespace Mage {

    class Color ;

    class MAGE_API Window  : ICopyDisable  // NOLINT(cppcoreguidelines-special-member-functions)
    {
        friend class Application ;

    public:
        ~Window() ;

        uint_fast32_t get_width()  const ;
        uint_fast32_t get_height() const ;

        void set_clear_color( const Color& c ) ;

        void clear_window() const ;

        void present() const ;

    private:
        Window( const char* title, bool full_screen,
            uint_fast32_t width, uint_fast32_t height, int_fast8_t swap_interval ) ;

        struct Impl ;
        Impl* _impl ;
    } ;

}
