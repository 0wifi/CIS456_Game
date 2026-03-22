#pragma once

#include "Core.h"

// Define convenient logging macros for engine and application loggers for warnings and errors.
#define LOG_E_WARN( ... )   ::Mage::Log::get().engine_warning     ( __VA_ARGS__ )
#define LOG_E_ERROR( ... )  ::Mage::Log::get().engine_error       ( __VA_ARGS__ )
#define LOG_WARN( ... )     ::Mage::Log::get().application_warning( __VA_ARGS__ )
#define LOG_ERROR( ... )    ::Mage::Log::get().application_error  ( __VA_ARGS__ )

// Define convenient logging macros for engine and application loggers for debug and info.
// But in release build, elid debug and info logging.
#ifdef NDEBUG
    #define LOG_E_DEBUG( ... )
    #define LOG_E_INFO( ... )
    #define LOG_DEBUG( ... )
    #define LOG_INFO( ... )
#else
    #define LOG_E_DEBUG( ... )  ::Mage::Log::get().engine_debug     ( __VA_ARGS__ )
    #define LOG_E_INFO( ... )   ::Mage::Log::get().engine_info      ( __VA_ARGS__ )
    #define LOG_DEBUG( ... )    ::Mage::Log::get().application_debug( __VA_ARGS__ )
    #define LOG_INFO( ... )     ::Mage::Log::get().application_info ( __VA_ARGS__ )
#endif

namespace Mage {

    class MAGE_API Log final
    {
    public:
        enum class Level { Debug = 0, Info = 1, Warning = 2, Error = 3 } ;

        static Log& get() ;

        ~Log() ;
        
        Log( Log&& ) = delete ;
        Log( Log const& ) = delete ;
        Log& operator=( Log&& ) = delete ;
        Log& operator=( Log const& ) = delete ;

        void set_engine_log_level( Level lvl ) ;

        void engine_debug( const char* msg, ... ) ;
        void engine_info( const char* msg, ... ) ;
        void engine_warning( const char* msg, ... ) ;
        void engine_error( const char* msg, ... ) ;

        void engine_console_sink_enable( bool enable ) ;
        void engine_file_sink_enable( bool enable ) ;
        void set_engine_file_sink_parameters(
            const char* log_file, uint32_t log_file_size, uint32_t log_files_to_keep ) ;

        void set_application_log_level( Level lvl ) ;

        void application_debug( const char* msg, ... ) ;
        void application_info( const char* msg, ... ) ;
        void application_warning( const char* msg, ... ) ;
        void application_error( const char* msg, ... ) ;

        void application_console_sink_enable( bool enable ) ;
        void application_file_sink_enable( bool enable ) ;
        void set_application_file_sink_parameters(
            const char* log_file, uint32_t log_file_size, uint32_t log_files_to_keep ) ;

    private:
        Log() ;

        inline static Log* _instance = nullptr ;

        struct Impl ;
        Impl* _impl = nullptr ;

    } ;
}
