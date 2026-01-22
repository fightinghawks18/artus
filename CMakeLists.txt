cmake_minimum_required(VERSION 3.31)
project(Artus)

set(CMAKE_CXX_STANDARD 17)

add_executable(Artus runtime/main.cpp)
target_link_libraries(Artus PRIVATE ArtusCore)

add_subdirectory(engine)
