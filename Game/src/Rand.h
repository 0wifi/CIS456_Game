// ReSharper disable CppClangTidyClangDiagnosticUniqueObjectDuplication
#pragma once

#include <map>
#include <mutex>
#include <random>
#include <string>
#include <shared_mutex>

class Rand final
{
public:
    Rand() = delete ;

    static void add_uniform_real_distribution(
        const std::string& name, float lower_bound, float upper_bound )
    {
        std::unique_lock<std::shared_mutex> lock{ _sync_object } ;
        _uniform_real_distro_map.insert_or_assign( 
            name, std::uniform_real_distribution<float>{ lower_bound, upper_bound } ) ;
    }

    static float get_uniform_real()
    {
        return _default_uniform_real_distro( get_engine() ) ;
    }

    static float get_uniform_real( const std::string& name )
    {
        std::shared_lock<std::shared_mutex> lock{ _sync_object } ;

        if( const auto it  = _uniform_real_distro_map.find( name ) ;
                       it != _uniform_real_distro_map.end() )
            return it->second( get_engine() ) ;

        return get_uniform_real() ;
    }

private:
    static std::mt19937& get_engine()
    {
        thread_local std::random_device rd             ;
        thread_local std::mt19937       engine{ rd() } ;
        return engine ;
    }

    static inline std::shared_mutex                                            _sync_object                                ;
    static inline std::map<std::string, std::uniform_real_distribution<float>> _uniform_real_distro_map                    ;
    static inline std::uniform_real_distribution<float>                        _default_uniform_real_distro { 0.0f, 1.0f } ;

} ;
