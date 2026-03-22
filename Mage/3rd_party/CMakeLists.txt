cmake_minimum_required( VERSION 3.8 )

# this suppress all warnings for 3rd party code
string(REGEX REPLACE "/W[1|2|3|4]" "/w" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
string(REGEX REPLACE "/W[1|2|3|4]" "/w" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_subdirectory( "spdlog" )
add_subdirectory( "SDL" )
add_subdirectory( "glew/build/cmake" )
add_subdirectory( "freetype" )
