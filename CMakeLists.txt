cmake_minimum_required(VERSION 3.0)
project(app)

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    src/api_gfx.cpp
    src/api_sys.cpp
    src/api_aux.cpp
    src/api_snd.cpp
    src/app.cpp
    src/font.cpp
    src/renderer.cpp
    src/texture.cpp
    src/tilemap.cpp
    src/tileset.cpp
    src/main.cpp
)

if(WIN32)
    add_compile_definitions(SOKOL_D3D11)
elseif(EMSCRIPTEN)
    add_compile_definitions(SOKOL_GLES2)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS
        "--preload-file ../data@data"
    )
endif()
