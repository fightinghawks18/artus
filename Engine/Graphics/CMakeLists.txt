## Uses Vulkan

find_package(Vulkan REQUIRED)

add_library(ArtusGraphics
        Source/Vulkan/Device.cpp
        Source/Vulkan/Surface.cpp
        Source/Vulkan/Resources/Image.cpp
        Source/Vulkan/Utils/Vulkan/Allocator.cpp
        Source/Vulkan/Resources/Buffer.cpp
        Source/Vulkan/Resources/Shader.cpp
        Source/Vulkan/Resources/CommandAllocator.cpp
        Source/Vulkan/Resources/CommandEncoder.cpp
        Source/Vulkan/Resources/GraphicsPipeline.cpp
        Source/Vulkan/Resources/PipelineLayout.cpp
        Source/Vulkan/Resources/DescriptorAllocator.cpp
        Source/Vulkan/Resources/ImageView.cpp
        Source/Vulkan/Resources/DescriptorSetLayout.cpp
        Source/Vulkan/Resources/DescriptorSet.cpp
)

target_include_directories(ArtusGraphics
        PUBLIC Include
)

target_link_libraries(ArtusGraphics
        PUBLIC ArtusCore
        PUBLIC ArtusMath
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
