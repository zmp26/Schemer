cmake_minimum_required(VERSION 3.10)
project(Schemer)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)

include_directories(${CAIRO_INCLUDE_DIRS})
link_directories(${CAIRO_LIBRARY_DIRS})

add_executable(Schemer
               src/main.cpp
               src/LevelScheme.cpp
               src/CairoRenderer.cpp
               src/SchemeParser.cpp
)

target_link_libraries(Schemer ${CAIRO_LIBRARIES})