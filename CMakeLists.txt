cmake_minimum_required( VERSION 3.8 )


# Enable Hot Reload for MSVC compilers if supported.
if( POLICY CMP0141 )
  cmake_policy( SET CMP0141 NEW )
  set( CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>" )
endif()


# project must come before stuff that depends on project settings
project( "Mage" VERSION 1.0.0 LANGUAGES CXX)


# Maximum warning output from MSVC/gcc
if( MSVC )
    add_definitions( -D_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS )
    string(REGEX REPLACE "/W[1|2|3|4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REGEX REPLACE "/W[1|2|3|4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
    add_compile_options( -Wall -Wextra -Wpedantic )
endif()


add_subdirectory( "Mage" )
add_subdirectory( "Game" )
