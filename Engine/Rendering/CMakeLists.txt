add_library(ArtusRendering
        Source/RenderDevice.cpp
        Source/RenderContext.cpp
        Source/Utils/GraphicsAllocator.cpp
)

target_include_directories(ArtusRendering PUBLIC
        Include
)

target_link_libraries(ArtusRendering
        PUBLIC ArtusGraphics
)