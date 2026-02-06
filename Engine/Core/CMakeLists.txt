add_library(ArtusCore)
target_include_directories(ArtusCore
        PUBLIC Include
)

if (APPLE)
    target_sources(ArtusCore PRIVATE
            Source/Apple/Window.mm
    )

    target_link_libraries(ArtusCore
            PRIVATE "-framework CoreFoundation"
            PRIVATE "-framework AppKit"
    )
elseif (WIN32)
    target_sources(ArtusCore PRIVATE
            Source/Windows/Window.cpp
    )
endif ()
