#include "Window.h"
#include "SDL2/SDL_syswm.h"

namespace Mage
{

    struct Window::Impl
    {
        SDL_Window*   window      =      nullptr ;
        SDL_GLContext gl_context  =      nullptr ;
        int           width       =            0 ;
        int           height      =            0 ;
        Color         clear_color = Color::black ;

        void construct( const char* title, bool full_screen = true,
            uint_fast32_t w = 0, uint_fast32_t h = 0, int_fast8_t swap_interval = 0 )
        {
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 ) ;
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 ) ;
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE ) ;

            Uint32 flags = SDL_WINDOW_OPENGL ;
            if( full_screen )
                flags |= SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_MOUSE_GRABBED ;

            window = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                static_cast<int>( w ), static_cast<int>( h ), flags ) ;

            if( window == nullptr )
                throw Exception( ( std::string( "Failed creating window: " )
                    + SDL_GetError() ).c_str() ) ;

            gl_context = SDL_GL_CreateContext( window ) ;

            glewExperimental = GL_TRUE ;
            glewInit() ;

            SDL_GetWindowSize( window, &width, &height ) ;

            SDL_GL_SetSwapInterval( swap_interval ) ;

            // Blending is needed to render text over a background
            glEnable( GL_BLEND ) ;
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) ;
        }
    } ;

    Window::Window( const char* title, bool full_screen,
        uint_fast32_t width, uint_fast32_t height, int_fast8_t swap_interval )
    {
        _impl = new Impl() ;
        _impl->construct( title, full_screen, width, height, swap_interval ) ;
        LOG_E_INFO( "Window created%s: '%s'; width: %d; height: %d; swap_interval: %d",
            full_screen ? " (full screen)" : "", title, width, height, swap_interval ) ;
    }

    Window::~Window()
    {
        SDL_DestroyWindow( _impl->window ) ;
        delete _impl ;
        LOG_E_INFO( "Window destroyed." ) ;
    }

    uint_fast32_t Window::get_width() const
    {
        return static_cast<uint_fast32_t>( _impl->width ) ;
    }

    uint_fast32_t Window::get_height() const
    {
        return static_cast<uint_fast32_t>( _impl->height ) ;
    }

    void Window::set_clear_color( const Color& c )
    {
        _impl->clear_color = c ;
    }

    void Window::clear_window() const
    {
        glClearColor( _impl->clear_color.r_float(),
            _impl->clear_color.g_float(), _impl->clear_color.b_float(), 1.0f ) ;
        glClear( GL_COLOR_BUFFER_BIT ) ;
    }

    void Window::present() const
    {
        SDL_GL_SwapWindow( _impl->window ) ;
    }

}
