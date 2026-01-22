## Uses Vulkan

find_package(Vulkan REQUIRED)

add_library(ArtusGraphics
        Source/Device.cpp
)

target_include_directories(ArtusGraphics
        PUBLIC Include
)

target_link_libraries(ArtusGraphics
        PUBLIC Vulkan::Vulkan
)