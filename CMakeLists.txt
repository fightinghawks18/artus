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

compile_slang_file_to_spirv(Runtime/Resources/Shaders/DefaultMesh_vs.slang Shaders)
compile_slang_file_to_spirv(Runtime/Resources/Shaders/DefaultMesh_ps.slang Shaders)

compile_slang_file_to_spirv(Runtime/Resources/Shaders/DefaultSkybox.slang Shaders)
compile_slang_file_to_spirv(Runtime/Resources/Shaders/FullscreenQuad.slang Shaders)

add_subdirectory(engine)
