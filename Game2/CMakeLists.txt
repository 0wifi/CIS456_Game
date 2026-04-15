cmake_minimum_required( VERSION 3.8 )

# --- Configuration ---
set( PROJECT_NAME_STR "Game2" )
# ---------------------

if( CMAKE_BUILD_TYPE STREQUAL "Release" )
    # set( EXECUTABLE_TYPE_WIN32 WIN32 )
endif()

add_executable(
    ${PROJECT_NAME_STR} ${EXECUTABLE_TYPE_WIN32}
    "src/main.cpp"
	"src/Game.cpp"
    "src/PlayerSystem.cpp"
    "src/CollisionSystem.cpp"
    "src/EnemySpawningSystem.cpp"
    "src/EnemyPointsTextSystem.cpp"
)

setup_force_build( ${PROJECT_NAME_STR} "src/main.cpp")

set_property( TARGET ${PROJECT_NAME_STR} PROPERTY LINKER_LANGUAGE CXX )
set_property( TARGET ${PROJECT_NAME_STR} PROPERTY CXX_STANDARD 20 )

target_link_libraries( ${PROJECT_NAME_STR} PUBLIC "Mage" )

# This copies any shared libraries that were built in the whole project
# to this target's output directory (after and only if the project has been built.)
add_custom_command( TARGET ${PROJECT_NAME_STR} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME_STR}> $<TARGET_FILE_DIR:${PROJECT_NAME_STR}>
    COMMAND_EXPAND_LISTS 
)

# This copies the resources (res) directory from the game source to the game binary output directory.
add_custom_command( TARGET ${PROJECT_NAME_STR} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
    COMMAND_EXPAND_LISTS 
)