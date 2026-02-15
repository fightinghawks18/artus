//
// Created by fightinghawks18 on 2/14/2026.
//


#ifndef ARTUS_RENDERING_GRAPHICS_ALLOCATOR_H
#define ARTUS_RENDERING_GRAPHICS_ALLOCATOR_H

#include "ResourceAllocator.h"
#include "Artus/Graphics/Vulkan/Resources/BindGroup.h"

#include <Artus/Graphics/Vulkan/Resources/Image.h>
#include <Artus/Graphics/Vulkan/Resources/ImageView.h>

#include <memory>

namespace Artus::Rendering {
    class RenderDevice;

    enum class ImageType {
        Color,
        DepthStencil
    };

    using ImageHandle = Handle<Graphics::Vulkan::Image>;
    using ImageViewHandle = Handle<Graphics::Vulkan::ImageView>;
    using ShaderHandle = Handle<Graphics::Vulkan::Shader>;
    using BufferHandle = Handle<Graphics::Vulkan::Buffer>;
    using GraphicsPipelineHandle = Handle<Graphics::Vulkan::GraphicsPipeline>;
    using PipelineLayoutHandle = Handle<Graphics::Vulkan::PipelineLayout>;
    using BindGroupLayoutHandle = Handle<Graphics::Vulkan::BindGroupLayout>;
    using BindGroupHandle = Handle<Graphics::Vulkan::BindGroup>;

    struct ImageAndViewHandle {
        ImageHandle img;
        ImageViewHandle view;

        operator Handle<Graphics::Vulkan::Image>() const { return img; }
        operator Handle<Graphics::Vulkan::ImageView>() const { return view; }
    };

    struct ImageCreateDesc {
        uint32_t width;
        uint32_t height;
        Graphics::Enums::Format format;
        Graphics::Flags::ImageUsage usage;

        ImageCreateDesc& SetWidth(const uint32_t imageWidth) {
            width = imageWidth;
            return *this;
        }

        ImageCreateDesc& SetHeight(const uint32_t imageFormat) {
            height = imageFormat;
            return *this;
        }

        ImageCreateDesc& SetFormat(const Graphics::Enums::Format imageFormat) {
            format = imageFormat;
            return *this;
        }

        ImageCreateDesc& SetUsage(const Graphics::Flags::ImageUsage imageUsage) {
            usage = imageUsage;
            return *this;
        }
    };

    struct ImageViewCreateDesc {
        ImageHandle image;
        ImageType type;
        Graphics::Enums::Format format;

        ImageViewCreateDesc& SetImage(const ImageHandle imageHandle) {
            image = imageHandle;
            return *this;
        }

        ImageViewCreateDesc& SetType(const ImageType imageType) {
            type = imageType;
            return *this;
        }

        ImageViewCreateDesc& SetFormat(const Graphics::Enums::Format imageFormat) {
            format = imageFormat;
            return *this;
        }
    };

    enum class VertexFormat {
        PositionUVColor
    };

    struct ShaderCreateDesc {
        std::string path;

        ShaderCreateDesc& SetShaderPath(const std::string& shaderPath) {
            path = shaderPath;
            return *this;
        }
    };

    struct BufferCreateDesc {
        size_t size;
        Graphics::Enums::BufferUsage usage;
        void* initData;

        BufferCreateDesc& SetSize(const size_t bufferSize) {
            size = bufferSize;
            return *this;
        }

        BufferCreateDesc& SetUsage(const Graphics::Enums::BufferUsage bufferUsage) {
            usage = bufferUsage;
            return *this;
        }

        BufferCreateDesc& SetInitialData(void* initialData) {
            initData = initialData;
            return *this;
        }
    };

    struct BindGroupLayoutCreateDesc : Graphics::Structs::BindGroupLayoutCreateDesc {
        BindGroupLayoutCreateDesc& AddBinding(const Graphics::Structs::BindGroupLayoutBinding& binding) {
            bindings.push_back(binding);
            return *this;
        }
    };

    struct BindGroupBinding {
        uint32_t binding;
        std::variant<BufferHandle, ImageViewHandle> resource;
    };

    struct BindGroupCreateDesc {
        BindGroupLayoutHandle bindGroupLayout;
        std::vector<BindGroupBinding> bindings;

        BindGroupCreateDesc& SetLayout(const BindGroupLayoutHandle bindGroupLayoutHandle) {
            bindGroupLayout = bindGroupLayoutHandle;
            return *this;
        }

        BindGroupCreateDesc& AddBinding(const BindGroupBinding& binding) {
            bindings.push_back(binding);
            return *this;
        }
    };

    struct PipelineLayoutCreateDesc {
        std::vector<BindGroupLayoutHandle> groupLayouts;

        PipelineLayoutCreateDesc& AddLayout(const BindGroupLayoutHandle bindGroupLayoutHandle) {
            groupLayouts.push_back(bindGroupLayoutHandle);
            return *this;
        }
    };

    struct GraphicsPipelineCreateDesc {
        ShaderHandle vertexShader;
        ShaderHandle pixelShader;
        PipelineLayoutHandle pipelineLayout;
        Graphics::Structs::GraphicsPipelineInputBinding binding;
        Graphics::Enums::CullMode cullMode;
        Graphics::Enums::DepthComparison depthCompare;
        Graphics::Enums::Format colorFormat;
        Graphics::Enums::Format depthFormat;

        GraphicsPipelineCreateDesc& SetLayout(const PipelineLayoutHandle pipelineLayoutHandle) {
            pipelineLayout = pipelineLayoutHandle;
            return *this;
        }

        GraphicsPipelineCreateDesc& SetVertexFormat(VertexFormat format) {
            binding = {};

            switch (format) {
            case VertexFormat::PositionUVColor: {
                binding.attributes.push_back(Graphics::Structs::GraphicsPipelineInputAttribute{
                    .location = 0,
                    .format = Graphics::Enums::Format::RGB32_Float,
                    .offset = offsetof(Graphics::Structs::Vertex3D, position)
                });

                binding.attributes.push_back(Graphics::Structs::GraphicsPipelineInputAttribute{
                    .location = 1,
                    .format = Graphics::Enums::Format::RG32_Float,
                    .offset = offsetof(Graphics::Structs::Vertex3D, uvCoords)
                });

                binding.attributes.push_back(Graphics::Structs::GraphicsPipelineInputAttribute{
                    .location = 2,
                    .format = Graphics::Enums::Format::RGBA32_Float,
                    .offset = offsetof(Graphics::Structs::Vertex3D, color)
                });
                break;
            }
            }
            return *this;
        }

        GraphicsPipelineCreateDesc& SetVertexFormat(const Graphics::Structs::GraphicsPipelineInputBinding& vertexFormat) {
            binding = vertexFormat;
            return *this;
        }

        GraphicsPipelineCreateDesc& SetCullingMode(const Graphics::Enums::CullMode cullingMode) {
            cullMode = cullingMode;
            return *this;
        }

        GraphicsPipelineCreateDesc& SetDepthComparingOp(const Graphics::Enums::DepthComparison depthComparisonOp) {
            depthCompare = depthComparisonOp;
            return *this;
        }

        GraphicsPipelineCreateDesc& SetColorFormat(const Graphics::Enums::Format format) {
            colorFormat = format;
            return *this;
        }

        GraphicsPipelineCreateDesc& SetDepthFormat(const Graphics::Enums::Format format) {
            depthFormat = format;
            return *this;
        }
    };

    class GraphicsAllocator {
    public:
        explicit GraphicsAllocator(RenderDevice& renderDevice);
        ~GraphicsAllocator();

        BufferHandle CreateBuffer(const BufferCreateDesc& bufferCreateDesc) const;
        ImageHandle CreateImage(const ImageCreateDesc& imageCreateDesc) const;
        ImageViewHandle CreateImageView(const ImageViewCreateDesc& imageViewCreateDesc) const;
        ImageAndViewHandle CreateImageAndView(const ImageCreateDesc& imageCreateDesc, ImageViewCreateDesc& imageViewCreateDesc) const;
        BindGroupLayoutHandle CreateBindGroupLayout(const BindGroupLayoutCreateDesc& bindGroupLayoutCreateDesc) const;
        BindGroupHandle CreateBindGroup(const BindGroupCreateDesc& bindGroupCreateDesc) const;
        PipelineLayoutHandle CreatePipelineLayout(const PipelineLayoutCreateDesc& pipelineLayoutCreateDesc) const;
        GraphicsPipelineHandle CreateGraphicsPipeline(const GraphicsPipelineCreateDesc& graphicsPipelineCreateDesc) const;
        ShaderHandle CreateShader(const ShaderCreateDesc& shaderCreateDesc) const;

        void DestroyBuffer(BufferHandle& bufferHandle) const;
        void DestroyImage(ImageHandle& imageHandle) const;
        void DestroyImageView(ImageViewHandle& imageViewHandle) const;
        void DestroyBindGroupLayout(BindGroupLayoutHandle& bindGroupLayoutHandle) const;
        void DestroyBindGroup(BindGroupHandle& bindGroupHandle) const;
        void DestroyPipelineLayout(PipelineLayoutHandle& pipelineLayoutHandle) const;
        void DestroyGraphicsPipeline(GraphicsPipelineHandle& graphicsPipelineHandle) const;
        void DestroyShader(ShaderHandle& shaderHandle) const;

        [[nodiscard]] ResourceAllocator<Graphics::Vulkan::Image>* GetImageAllocator() const { return mImageAllocator.get(); }
        [[nodiscard]] ResourceAllocator<Graphics::Vulkan::ImageView>* GetImageViewAllocator() const { return mImageViewAllocator.get(); }

    private:
        RenderDevice& mRenderDevice;

        std::unique_ptr<ResourceAllocator<Graphics::Vulkan::Buffer>> mBufferAllocator;
        std::unique_ptr<ResourceAllocator<Graphics::Vulkan::Image>> mImageAllocator;
        std::unique_ptr<ResourceAllocator<Graphics::Vulkan::ImageView>> mImageViewAllocator;
        std::unique_ptr<ResourceAllocator<Graphics::Vulkan::BindGroupLayout>> mBindGroupLayoutAllocator;
        std::unique_ptr<ResourceAllocator<Graphics::Vulkan::BindGroup>> mBindGroupAllocator;
        std::unique_ptr<ResourceAllocator<Graphics::Vulkan::PipelineLayout>> mPipelineLayoutAllocator;
        std::unique_ptr<ResourceAllocator<Graphics::Vulkan::GraphicsPipeline>> mGraphicsPipelineAllocator;
        std::unique_ptr<ResourceAllocator<Graphics::Vulkan::Shader>> mShaderAllocator;
    };
}

#endif // ARTUS_RENDERING_GRAPHICS_ALLOCATOR_H