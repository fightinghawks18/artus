cmake_minimum_required(VERSION 3.31)
project(Artus)

set(CMAKE_CXX_STANDARD 20)

add_executable(Artus runtime/main.cpp)
target_link_libraries(Artus
        PRIVATE ArtusCore
        PRIVATE ArtusGraphics
)

add_subdirectory(engine)
