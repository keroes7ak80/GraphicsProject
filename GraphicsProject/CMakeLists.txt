set(CMAKE_LEGACY_CYGWIN_WIN32 OFF)

cmake_minimum_required (VERSION 3.0)

project (GraphicsProject)

find_package (OpenGL REQUIRED)


file(GLOB_RECURSE SOURCES      ./*.cpp)
file(GLOB_RECURSE HEADERS      ./*.h)

add_executable (trial ${SOURCES} ${HEADERS})

target_link_libraries (trial -lGL -lGLEW -lglfw -lSOIL -lassimp)
