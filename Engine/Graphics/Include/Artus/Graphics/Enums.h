//
// Created by fightinghawks18 on 2/14/2026.
//


#ifndef ARTUS_GRAPHICS_ENUMS_H
#define ARTUS_GRAPHICS_ENUMS_H

namespace Artus::Graphics::Enums {
    enum class BindGroupLayoutBindingType {
        Buffer,
        ImageView
    };

    enum class BufferUsage {
        Vertex, ///< Buffer is used to store vertices for an object
        Index, ///< Buffer is used to store indices for an object
        Shader, ///< Buffer is read-only in shaders and stores small amounts of data
        ShaderStorage, ///< Buffer is read/write in shaders and stores large amounts of data
    };

    enum class CullMode {
        Never, ///< Never cull faces
        Back, ///< Cull back faces
        Front, ///< Cull front faces
        BackAndFront, ///< Cull front and back faces
    };

    enum class ImageType {
        Image2D,
        Image3D
    };

    enum class ImageViewType {
        ImageView2D,
        ImageView3D
    };

    enum class ImageState {
        Unknown,
        Color,
        DepthStencil,
        ShaderAccess,
        Presentable
    };

    enum class RenderingAttachmentLoadStoreOp {
        LoadThenStore,
        ClearThenStore,
    };

    enum class RenderingAttachmentType {
        Color,
        DepthStencil,
        Shader
    };

    enum class DepthComparison {
        Never, ///< Never compare depth

        Less, ///< Fragment depth is less than screen pixel depth
        LessOrEqual, ///< Fragment depth is less or equal to screen pixel depth

        Greater, ///< Fragment depth is greater than screen pixel depth
        GreaterOrEqual, ///< Fragment depth is greater or equal to screen pixel depth

        Always, ///< Fragment depth is always accepted
    };

    enum class ShaderStage {
        Vertex,
        Pixel,
        Compute
    };

    enum class Format {
        Unknown,

        RGBA8_SInt,
        RGBA8_SRgb,
        RGBA8_UNorm,
        RGBA8_SNorm,

        BGRA8_SInt,
        BGRA8_SRgb,
        BGRA8_UNorm,
        BGRA8_SNorm,

        RG32_Float,
        RGB32_Float,
        RGBA32_Float,

        D16_UNorm_S8_UInt,
        D32_SFloat_S8_UInt,
    };
}

#endif // ARTUS_GRAPHICS_ENUMS_H