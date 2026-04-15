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
 

function( setup_force_build TARGET_NAME SOURCE_FILE_PATH )
    get_target_property( TARGET_SRC_DIR ${TARGET_NAME} SOURCE_DIR )
    set( CUSTOM_TARGET_NAME "force_${TARGET_NAME}_build" )

    # Combine the path once and wrap in quotes for safety
    set( FULL_PATH "${TARGET_SRC_DIR}/${SOURCE_FILE_PATH}" )

    if( WIN32 )
        add_custom_target( ${CUSTOM_TARGET_NAME} ALL
            # Note the \" around the path so PowerShell sees it as one string
            COMMAND powershell "(ls \"${FULL_PATH}\").LastWriteTime = Get-Date"
            COMMENT "Force-touching ${SOURCE_FILE_PATH} to trigger build for ${TARGET_NAME}" )
    else()
        add_custom_target( ${CUSTOM_TARGET_NAME} ALL
            # Always quote paths in CMake commands
            COMMAND ${CMAKE_COMMAND} -E touch "${FULL_PATH}"
            COMMENT "Force-touching ${SOURCE_FILE_PATH} to trigger build for ${TARGET_NAME}" )
    endif()

    add_dependencies( ${TARGET_NAME} ${CUSTOM_TARGET_NAME} )
endfunction()

add_subdirectory( "Mage" )
add_subdirectory( "Game" )
add_subdirectory( "Game2" )
