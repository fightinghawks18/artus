## Uses Vulkan

find_package(Vulkan REQUIRED)

add_library(ArtusGraphics
        Source/Device.cpp
        Source/Surface.cpp
        Source/Resources/Image.cpp
        Source/Utils/Vulkan/Allocator.cpp
        Source/Resources/Buffer.cpp
        Source/Resources/Shader.cpp
        Source/Resources/CommandAllocator.cpp
        Source/Resources/CommandEncoder.cpp
)

target_include_directories(ArtusGraphics
        PUBLIC Include
)

target_link_libraries(ArtusGraphics
        PUBLIC ArtusCore
        PUBLIC Vulkan::Vulkan
)

if (APPLE)
    target_sources(ArtusGraphics PRIVATE
            Source/Utils/Metal/Layer.mm
    )

    target_link_libraries(ArtusGraphics
            PRIVATE "-framework QuartzCore"
    )
endif ()
