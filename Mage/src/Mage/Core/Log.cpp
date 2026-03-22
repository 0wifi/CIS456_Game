#include "Log.h"

#define FORMAT_LOG_ENTRY( msg, formatted_msg ) \
    char formatted_msg[ 4096 ] ; \
    va_list ap ; \
    va_start( ap, msg ) ; \
    vsnprintf( formatted_msg, sizeof( formatted_msg ), msg, ap ) ; \
    va_end( ap ) ;

namespace Mage {

    struct Log::Impl
    {
        void create_engine_logger()
        {
            std::lock_guard lock( sync_object ) ;
            if( engine_logger == nullptr )
            {
                auto sinks = create_sinks( engine_console_sink_enabled, engine_file_sink_enabled,
                    engine_log_file, engine_log_file_size, engine_log_files_to_keep ) ;
                engine_logger = std::make_shared<spdlog::logger>( "E", sinks.begin(), sinks.end() ) ;
                engine_logger->set_level( spdlog::level::debug ) ;
                apply_logger_format( engine_logger ) ;
            }
        }

        void create_application_logger()
        {
            std::lock_guard lock( sync_object ) ;
            if( application_logger == nullptr )
            {
                auto sinks = create_sinks( application_console_sink_enabled, application_file_sink_enabled,
                    application_log_file, application_log_file_size, application_log_files_to_keep ) ;
                application_logger = std::make_shared<spdlog::logger>( "A", sinks.begin(), sinks.end() ) ;
                application_logger->set_level( spdlog::level::debug ) ;
                apply_logger_format( application_logger ) ;
            }
        }

        static std::vector<spdlog::sink_ptr> create_sinks( bool console_sink_enabled, bool file_sink_enabled,
            const std::string& log_file, uint32_t log_file_size, uint32_t log_files_to_keep )
        {
            std::vector<spdlog::sink_ptr> sinks{} ;
            if( console_sink_enabled )
            {
                auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>() ;
                sinks.push_back( console_sink ) ;
            }
            if( file_sink_enabled )
            {
                auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                    log_file, log_file_size, log_files_to_keep ) ;
                sinks.push_back( file_sink ) ;
            }
            if( sinks.empty() )
            {
                auto null_sink = std::make_shared<spdlog::sinks::null_sink_mt>() ;
                sinks.push_back( null_sink ) ;
            }
            return sinks ;
        }

        static void apply_logger_format( const std::shared_ptr<spdlog::logger>& logger )
        {
            logger->set_pattern( "[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v" ) ;
        }

        std::mutex  sync_object                                     ;
        Level       engine_log_level                 = Level::Debug ;
        Level       application_log_level            = Level::Debug ;
        bool        engine_console_sink_enabled      =         true ;
        bool        engine_file_sink_enabled         =        false ;
        bool        application_console_sink_enabled =         true ;
        bool        application_file_sink_enabled    =        false ;
        std::string engine_log_file                                 ;
        uint32_t    engine_log_file_size             =            0 ;
        uint32_t    engine_log_files_to_keep         =            0 ;
        std::string application_log_file                            ;
        uint32_t    application_log_file_size        =            0 ;
        uint32_t    application_log_files_to_keep    =            0 ;

        std::shared_ptr<spdlog::sinks::stdout_color_sink_mt>  engine_console_sink      = nullptr ;
        std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> engine_file_sink         = nullptr ;
        std::shared_ptr<spdlog::sinks::stdout_color_sink_mt>  application_console_sink = nullptr ;
        std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> application_file_sink    = nullptr ;
        std::shared_ptr<spdlog::logger>                       engine_logger            = nullptr ;
        std::shared_ptr<spdlog::logger>                       application_logger       = nullptr ;
    } ;

    Log::Log()
    {
        _impl = new Impl() ;
    }

    Log::~Log()
    {
        delete _impl ;
        spdlog::shutdown() ;
    }

    Log& Log::get()
    {
        if( _instance == nullptr )
            _instance = new Log() ;
        return *_instance ;
    }

    void Log::set_engine_log_level( Level lvl )
    {
        std::lock_guard lock( _impl->sync_object ) ;
        _impl->engine_log_level = lvl ;
    }

    void Log::set_application_log_level( Level lvl )
    {
        std::lock_guard lock( _impl->sync_object ) ;
        _impl->application_log_level = lvl ;
    }

    void Log::engine_debug( const char* msg, ... )
    {
        if( _impl->engine_log_level > Level::Debug )
            return ;

        FORMAT_LOG_ENTRY( msg, formatted_msg )
        _impl->create_engine_logger() ;
        _impl->engine_logger->debug( formatted_msg ) ;
    }

    void Log::engine_info( const char* msg, ... )
    {
        if( _impl->engine_log_level > Level::Info )
            return ;

        FORMAT_LOG_ENTRY( msg, formatted_msg )
        _impl->create_engine_logger() ;
        _impl->engine_logger->info( formatted_msg ) ;
    }

    void Log::engine_warning( const char* msg, ... )
    {
        if( _impl->engine_log_level > Level::Warning )
            return ;

        FORMAT_LOG_ENTRY( msg, formatted_msg )
        _impl->create_engine_logger() ;
        _impl->engine_logger->warn( formatted_msg ) ;
    }

    void Log::engine_error( const char* msg, ... )
    {
        FORMAT_LOG_ENTRY( msg, formatted_msg )
        _impl->create_engine_logger() ;
        _impl->engine_logger->error( formatted_msg ) ;
    }

    void Log::application_debug( const char* msg, ... )
    {
        if( _impl->application_log_level > Level::Debug )
            return ;

        FORMAT_LOG_ENTRY( msg, formatted_msg )
        _impl->create_application_logger() ;
        _impl->application_logger->debug( formatted_msg ) ;
    }

    void Log::application_info( const char* msg, ... )
    {
        if( _impl->application_log_level > Level::Info )
            return ;

        FORMAT_LOG_ENTRY( msg, formatted_msg )
        _impl->create_application_logger() ;
        _impl->application_logger->info( formatted_msg ) ;
    }

    void Log::application_warning( const char* msg, ... )
    {
        if( _impl->application_log_level > Level::Warning )
            return ;

        FORMAT_LOG_ENTRY( msg, formatted_msg )
        _impl->create_application_logger() ;
        _impl->application_logger->warn( formatted_msg ) ;
    }

    void Log::application_error( const char* msg, ... )
    {
        FORMAT_LOG_ENTRY( msg, formatted_msg )
        _impl->create_application_logger() ;
        _impl->application_logger->error( formatted_msg ) ;
    }

    void Log::engine_console_sink_enable( bool enable )
    {
        std::lock_guard lock( _impl->sync_object ) ;

        if( enable == _impl->engine_console_sink_enabled )
            return ;

        _impl->engine_console_sink_enabled = enable ;
        _impl->engine_logger = nullptr ;
    }

    void Log::engine_file_sink_enable( bool enable )
    {
        std::lock_guard lock( _impl->sync_object ) ;

        if( enable == _impl->engine_file_sink_enabled )
            return ;

        //if( enable && _impl->engine_log_file_size < 1 )
        //    throw Exception(
        //        "[Log::enable_engine_file_sink] A call to"
        //        " Log::set_engine_file_sink_parameters( ... )"
        //        " must precede any call to Log::enable_engine_file_sink()." ) ;

        _impl->engine_file_sink_enabled = enable ;
        _impl->engine_logger = nullptr ;
    }

    void Log::set_engine_file_sink_parameters(
        const char* log_file, uint32_t log_file_size, uint32_t log_files_to_keep )
    {
        std::lock_guard lock( _impl->sync_object ) ;

        //if( log_file_size < 1048576 )
        //    throw Exception(
        //        "[Log::set_engine_file_sink_parameters] log_file_size"
        //        "  must be at least 1MB (1048576)." ) ;

        //if( log_files_to_keep < 1 )
        //    throw Exception(
        //        "[Log::set_engine_file_sink_parameters] log_files_to_keep"
        //        "  must be at 1." ) ;

        _impl->engine_log_file = log_file ;
        _impl->engine_log_file_size = log_file_size ;
        _impl->engine_log_files_to_keep = log_files_to_keep ;
    }

    void Log::application_console_sink_enable( bool enable )
    {
        std::lock_guard lock( _impl->sync_object ) ;

        if( enable == _impl->application_console_sink_enabled )
            return ;

        _impl->application_console_sink_enabled = enable ;
        _impl->application_logger = nullptr ;
    }

    void Log::application_file_sink_enable( bool enable )
    {
        std::lock_guard lock( _impl->sync_object ) ;

        if( enable == _impl->application_file_sink_enabled )
            return ;

        //if( enable && _impl->application_log_file_size < 1 )
        //    throw Exception(
        //        "[Log::enable_application_file_sink] A call to"
        //        " Log::set_application_file_sink_parameters( ... )"
        //        " must precede any call to Log::enable_application_file_sink()." ) ;

        _impl->application_file_sink_enabled = enable ;
        _impl->application_logger = nullptr ;
    }

    void Log::set_application_file_sink_parameters(
        const char* log_file, uint32_t log_file_size, uint32_t log_files_to_keep )
    {
        std::lock_guard lock( _impl->sync_object ) ;

        //if( log_file_size < 1048576 )
        //    throw Exception(
        //        "[Log::set_application_file_sink_parameters] log_file_size"
        //        "  must be at least 1MB (1048576)." ) ;

        //if( log_files_to_keep < 1 )
        //    throw Exception(
        //        "[Log::set_application_file_sink_parameters] log_files_to_keep"
        //        "  must be at 1." ) ;

        _impl->application_log_file = log_file ;
        _impl->application_log_file_size = log_file_size ;
        _impl->application_log_files_to_keep = log_files_to_keep ;
    }
}
