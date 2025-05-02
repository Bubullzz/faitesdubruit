cmake_minimum_required(VERSION 3.16)
project(faitesdubruit)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
        main.cpp
        sources/voronoi.cpp
        libz/stb_image_write_bindings.cpp
)

set(HEADERS
        includes/image.hh
        includes/perlin.hh
        includes/utils.hh
        includes/vec2.hh
        includes/voronoi.hh
)

add_executable(faitesdubruit ${SOURCES} ${HEADERS})
