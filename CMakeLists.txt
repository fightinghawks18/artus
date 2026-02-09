cmake_minimum_required(VERSION 3.31)
project(Artus)

set(CMAKE_CXX_STANDARD 20)

include (CMake/slang.cmake)

add_executable(Artus Runtime/Source/Main.cpp)
target_link_libraries(Artus
        PRIVATE ArtusCore
        PRIVATE ArtusGraphics
        PRIVATE ArtusMath
)

compile_slang_file_to_spirv(Runtime/Resources/Shaders/PS_Default.slang Shaders)
compile_slang_file_to_spirv(Runtime/Resources/Shaders/VS_Default.slang Shaders)
compile_slang_file_to_spirv(Runtime/Resources/Shaders/VS_Fullscreen.slang Shaders)
compile_slang_file_to_spirv(Runtime/Resources/Shaders/PS_Skybox.slang Shaders)

add_subdirectory(engine)
