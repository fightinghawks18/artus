//
// Created by fightinghawks18 on 2/14/2026.
//


#ifndef ARTUS_GRAPHICS_STRUCTS_H
#define ARTUS_GRAPHICS_STRUCTS_H

#include "Enums.h"
#include "Flags.h"

#include <Artus/Math/Matrix4.h>
#include <Artus/Math/Color.h>

#include <array>
#include <variant>
#include <vector>
#include <string>

namespace Artus::Core {
    class Window;
}

namespace Artus::Graphics::Vulkan {
    class Image;
    class ImageView;
    class Buffer;
    class BindGroupLayout;
    class Shader;
    class PipelineLayout;
    class GraphicsPipeline;
}

namespace Artus::Graphics::Structs {
    struct SurfaceCreateDesc {
        Core::Window* window;
    };

    struct SurfaceFrameInfo {
        Vulkan::Image* colorImage;
        Vulkan::ImageView* colorView;

        Vulkan::Image* depthImage;
        Vulkan::ImageView* depthView;
    };

    struct BindGroupBinding {
        uint32_t binding;
        std::variant<Vulkan::Buffer*, Vulkan::ImageView*> resource;
    };

    struct BindGroupCreateDesc {
        Vulkan::BindGroupLayout* layout;
        std::vector<BindGroupBinding> bindings;
    };

    struct BindGroupLayoutBinding {
        Enums::ShaderStage visibility;
        uint32_t binding;
        Enums::BindGroupLayoutBindingType type;
    };

    struct BindGroupLayoutCreateDesc {
        std::vector<BindGroupLayoutBinding> bindings;
    };

    struct BufferCreateDesc {
        size_t size;
        Enums::BufferUsage usage;
    };

    struct Rectangle {
        int32_t x, y;
        uint32_t width, height;
    };

    struct Extent {
        uint32_t width, height;
    };

    struct Extent3D {
        uint32_t width, height, depth;
    };

    struct Viewport {
        float x, y, width, height;
        float minDepth, maxDepth;
    };

    struct Vertex2D {
        std::array<float, 2> position;
        std::array<float, 2> uvCoords;
        std::array<float, 4> color;
    };

    struct Vertex3D {
        std::array<float, 3> position;
        std::array<float, 2> uvCoords;
        std::array<float, 4> color;
    };

    struct ModelData {
        Math::Matrix4 model;
    };

    struct CameraData {
        Math::Matrix4 view;
        Math::Matrix4 projection;
    };

    struct RenderingDepthStencilClear {
        float depth;
        uint32_t stencil;
    };

    struct RenderingAttachment {
        Vulkan::ImageView* view = nullptr;
        Enums::RenderingAttachmentType type = Enums::RenderingAttachmentType::Color;
        Enums::RenderingAttachmentLoadStoreOp lsOp = Enums::RenderingAttachmentLoadStoreOp::ClearThenStore;
        std::variant<Math::Color, RenderingDepthStencilClear> clear;
    };

    struct RenderingPass {
        std::vector<RenderingAttachment> attachments;
        Rectangle renderArea;
    };

    struct GraphicsPipelineInputAttribute {
        uint32_t location;
        Enums::Format format;
        uint32_t offset;
    };

    struct GraphicsPipelineInputBinding {
        uint32_t slot;
        uint32_t stride;
        std::vector<GraphicsPipelineInputAttribute> attributes;
    };

    struct GraphicsPipelineCreateDesc {
        Vulkan::Shader* vertexStage;
        Vulkan::Shader* pixelStage;

        std::vector<GraphicsPipelineInputBinding> bindings;
        std::vector<Enums::Format> colorFormats;
        Enums::Format depthFormat;
        Enums::Format stencilFormat;
        Vulkan::PipelineLayout* pipelineLayout;
        Enums::DepthComparison depthCompare;
    };

    struct PipelineLayoutCreateDesc {
        std::vector<Vulkan::BindGroupLayout*> groupLayouts;
    };

    struct ShaderCreateDesc {
        std::string path;
    };

    struct ImageCreateDesc {
        Enums::Format format;
        Enums::ImageType type;
        Extent3D extent;
        Flags::ImageUsage usage;
        uint32_t layerCount;
        uint32_t levelCount;
    };

    struct ImageViewCreateDesc {
        Vulkan::Image* image;
        Enums::Format format;
        Enums::ImageViewType type;
        Flags::ImageAspect aspectMask;
        uint32_t baseLayer;
        uint32_t layerCount;
        uint32_t baseLevel;
        uint32_t levelCount;
    };
}

#endif // ARTUS_GRAPHICS_STRUCTS_H