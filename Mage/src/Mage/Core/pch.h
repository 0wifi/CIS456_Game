#pragma once

#include <any>
#include <map>
#include <set>
#include <array>
#include <mutex>
#include <atomic>
#include <memory>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <utility>
#include <iostream>
#include <functional>
#include <algorithm>
#include <typeindex>
#include <functional>
#include <unordered_map>
#include <unordered_set>

#pragma warning( push, 0 )

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/null_sink.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#ifdef _WIN32
    #include <Windows.h>
#endif

#define NO_SDL_GLEXT
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#include <GLM/gtx/quaternion.hpp>
#include <GLM/gtx/euler_angles.hpp>

#include "stb_image.h"

#pragma warning( pop )
