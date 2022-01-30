cmake_minimum_required(VERSION 3.0)
project(app)

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME}
    src/api_gfx.cpp
    src/api_sys.cpp
    src/app.cpp
    src/font.cpp
    src/renderer.cpp
    src/tilemap.cpp
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
  OpenGL::GL
)
