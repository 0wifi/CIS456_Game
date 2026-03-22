 cmake_minimum_required( VERSION 3.8 )

add_library(
 "Mage" SHARED
 "src/Mage/Mage.h"
 "src/Mage/Core/Core.h"
 "src/Mage/Core/Application.h"
 "src/Mage/Core/Application.cpp"
 "src/Mage/Core/Color.h"
 "src/Mage/Core/Color.cpp"
 "src/Mage/Core/Core.cpp"
 "src/Mage/Core/EventEnums.h"
 "src/Mage/Core/EventListenerBase.h"
 "src/Mage/Core/EventManager.h"
 "src/Mage/Core/EventManager.cpp"
 "src/Mage/Core/Exception.h"
 "src/Mage/Core/Log.h" 
 "src/Mage/Core/Log.cpp"
 "src/Mage/Core/Window.h"
 "src/Mage/Core/Window.cpp"
 "src/Mage/IO/IO.h"
 "src/Mage/IO/Controller.h"
 "src/Mage/IO/Controller.cpp"
 "src/Mage/Renderer/Renderer.h"
 "src/Mage/Renderer/Font.h"
 "src/Mage/Renderer/Font.cpp"
 
 
 "src/Mage/Renderer/Shader.h"
 "src/Mage/Renderer/Shader.cpp"
 "src/Mage/Renderer/Sprite.h"
 "src/Mage/Renderer/Sprite.h"
 "src/Mage/Renderer/Sprite.cpp"
 "src/Mage/Renderer/SpriteRenderer.h"
 "src/Mage/Renderer/SpriteRenderer.cpp"
 
 
 "src/Mage/Renderer/TextRenderer.h"
 "src/Mage/Renderer/TextRenderer.cpp"
 "src/Mage/Scene/Scene.h"
 "src/Mage/Scene/ComponentVector.h"
 "src/Mage/Scene/ComponentManager.h"
 "src/Mage/Scene/ComponentManager.cpp"
 "src/Mage/Scene/Entity.h"
 "src/Mage/Scene/EntityManager.h"
 "src/Mage/Scene/EntityManager.cpp"
 "src/Mage/Scene/EntityList.h"
 "src/Mage/Scene/EntityList.cpp"
 "src/Mage/Scene/SystemManager.h"
 "src/Mage/Scene/SystemManager.cpp"
 "src/Mage/Scene/System.h"
 "src/Mage/Scene/System.cpp"
 "src/Mage/Scene/SystemList.h"
 "src/Mage/Scene/SystemList.cpp"
 "src/Mage/Core/Exception.cpp"  )

target_include_directories(
 "Mage" PUBLIC
 "./src"
 "./3rd_party/spdlog/include"
 "./3rd_party/SDL/include"
 "./3rd_party/glew/include"
 "./3rd_party/freetype/include"
 "./3rd_party/glm/include"
 "./3rd_party/stb_image/include"
 "./"
)

target_precompile_headers( "Mage" PRIVATE "src/Mage/Core/pch.h" )

find_package( OpenGL REQUIRED )

add_compile_definitions( MAGE_LIBRARY_BUILD )

set_property( TARGET "Mage" PROPERTY LINKER_LANGUAGE CXX )
set_property( TARGET "Mage" PROPERTY CXX_STANDARD 20 )

set( SDL_SHARED OFF )
set( SDL_STATIC ON )
set( GLEW_USE_STATIC_LIBS ON )
set( BUILD_SHARED_LIBS OFF ) # freetype

add_subdirectory( "3rd_party" )

target_link_libraries( "Mage" PUBLIC "SDL2::SDL2" )
target_link_libraries( "Mage" PRIVATE ${OPENGL_LIBRARIES} )
target_link_libraries( "Mage" PRIVATE "glew_s" )
target_link_libraries( "Mage" PRIVATE "freetype" )
