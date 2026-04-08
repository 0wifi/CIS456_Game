cmake_minimum_required( VERSION 3.8 )

if( CMAKE_BUILD_TYPE STREQUAL "Release" )
    set( EXECUTABLE_TYPE_WIN32 WIN32 )
endif()

add_executable(
 "Game" ${EXECUTABLE_TYPE_WIN32}
 "src/main.cpp"
 "src/Game.cpp"
 "src/PlayerSystem.cpp"
 
 "src/CollisionSystem.cpp" "src/DestructionNotificationSystem.h" "src/EnemySpawningSystem.h" "src/EnemySpawningSystem.cpp" "src/SpriteRenderingSystem.h"  "src/EnemyPointsTextSystem.h" "src/EnemyPointsTextSystem.cpp")

set_property( TARGET "Game" PROPERTY LINKER_LANGUAGE CXX )
set_property( TARGET "Game" PROPERTY CXX_STANDARD 20) 

target_link_libraries( "Game" PUBLIC "Mage" )

# This updates the modified date/time of the specified source file to force a
#  build to happen, which in turn will force the post build step of copying any
#  built DLLs to this target's output directory.
set( SOURCE_FILE_TO_MODIFY "/src/main.cpp" )
get_target_property(GAME_SOURCE_DIR Game SOURCE_DIR)
if( WIN32 )
    add_custom_target(force_game_to_build ALL
        COMMAND powershell "(ls ${GAME_SOURCE_DIR}${SOURCE_FILE_TO_MODIFY}).LastWriteTime = Get-Date")
else()
    add_custom_target(force_game_to_build ALL
        COMMAND ${CMAKE_COMMAND} -E touch "${GAME_SOURCE_DIR}/main.cpp")
endif()
add_dependencies("Game" force_game_to_build)

# This copies any shared libraries that were built in the whole project
#  to this target's output directory (after and only if the project has been built.)
add_custom_command( TARGET "Game" POST_BUILD
 COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Game> $<TARGET_FILE_DIR:Game>
 COMMAND_EXPAND_LISTS)

# This copies the resources (res) directory from the game source to the game binary output directory.
 add_custom_command( TARGET "Game" POST_BUILD
 COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
 COMMAND_EXPAND_LISTS)
