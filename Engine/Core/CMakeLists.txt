add_library(ArtusCore)
target_include_directories(ArtusCore
        PUBLIC Include
)

if (APPLE)
    target_sources(ArtusCore PRIVATE
            Source/Apple/Platform.mm
    )

    target_link_libraries(ArtusCore
            PRIVATE "-framework CoreFoundation"
            PRIVATE "-framework AppKit"
    )
endif ()
