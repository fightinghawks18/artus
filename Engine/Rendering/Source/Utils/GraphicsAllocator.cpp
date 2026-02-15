//
// Created by fightinghawks18 on 2/14/2026.
//

#include "Artus/Rendering/Utils/GraphicsAllocator.h"

#include "Artus/Rendering/RenderDevice.h"

namespace Artus::Rendering {
    GraphicsAllocator::GraphicsAllocator(RenderDevice& renderDevice) : mRenderDevice(renderDevice) {
        mBufferAllocator = std::make_unique<ResourceAllocator<Graphics::Vulkan::Buffer>>();
        mImageAllocator = std::make_unique<ResourceAllocator<Graphics::Vulkan::Image>>();
        mImageViewAllocator = std::make_unique<ResourceAllocator<Graphics::Vulkan::ImageView>>();
        mBindGroupLayoutAllocator = std::make_unique<ResourceAllocator<Graphics::Vulkan::BindGroupLayout>>();
        mBindGroupAllocator = std::make_unique<ResourceAllocator<Graphics::Vulkan::BindGroup>>();
        mPipelineLayoutAllocator = std::make_unique<ResourceAllocator<Graphics::Vulkan::PipelineLayout>>();
        mGraphicsPipelineAllocator = std::make_unique<ResourceAllocator<Graphics::Vulkan::GraphicsPipeline>>();
        mShaderAllocator = std::make_unique<ResourceAllocator<Graphics::Vulkan::Shader>>();
    }

    GraphicsAllocator::~GraphicsAllocator() {
        mGraphicsPipelineAllocator.reset();
        mShaderAllocator.reset();
        mPipelineLayoutAllocator.reset();

        mBufferAllocator.reset();
        mBindGroupAllocator.reset();
        mBindGroupLayoutAllocator.reset();

        mImageViewAllocator.reset();
        mImageAllocator.reset();
    }

    BufferHandle GraphicsAllocator::CreateBuffer(const BufferCreateDesc& bufferCreateDesc) const {
        auto buf = mBufferAllocator->Allocate(mRenderDevice.GetRHI()->CreateBuffer(Graphics::Structs::BufferCreateDesc {
            .size = bufferCreateDesc.size,
            .usage = bufferCreateDesc.usage
        }));

        if (bufferCreateDesc.initData)
            mBufferAllocator->Get(buf)->Map(bufferCreateDesc.size, 0, &bufferCreateDesc.initData);
        return buf;
    }

    ImageHandle GraphicsAllocator::CreateImage(const ImageCreateDesc& imageCreateDesc) const {
        auto img = mImageAllocator->Allocate(mRenderDevice.GetRHI()->CreateImage(Graphics::Structs::ImageCreateDesc{
            .format = imageCreateDesc.format,
            .type = Graphics::Enums::ImageType::Image2D,
            .extent = {imageCreateDesc.width, imageCreateDesc.height, 1},
            .usage = imageCreateDesc.usage,
            .layerCount = 1,
            .levelCount = 1
        }));

        return img;
    }

    ImageViewHandle GraphicsAllocator::CreateImageView(const ImageViewCreateDesc& imageViewCreateDesc) const {
        auto aspectMask = Graphics::Flags::ImageAspect::None;
        switch (imageViewCreateDesc.type) {
        case ImageType::Color:
            aspectMask = Graphics::Flags::ImageAspect::Color;
            break;
        case ImageType::DepthStencil:
            aspectMask = Graphics::Flags::ImageAspect::Depth | Graphics::Flags::ImageAspect::Stencil;
            break;
        }

        const auto view = mImageViewAllocator->Allocate(mRenderDevice.GetRHI()->CreateImageView(Graphics::Structs::ImageViewCreateDesc {
            .image = mImageAllocator->Get(imageViewCreateDesc.image),
            .format = imageViewCreateDesc.format,
            .type = Graphics::Enums::ImageViewType::ImageView2D,
            .aspectMask = aspectMask,
            .baseLayer = 0,
            .layerCount = 1,
            .baseLevel = 0,
            .levelCount = 1
        }));

        return view;
    }

    ImageAndViewHandle GraphicsAllocator::CreateImageAndView(const ImageCreateDesc& imageCreateDesc,
        ImageViewCreateDesc& imageViewCreateDesc) const {
        const auto img = CreateImage(imageCreateDesc);
        imageViewCreateDesc.SetImage(img).SetFormat(imageCreateDesc.format);
        return {img, CreateImageView(imageViewCreateDesc)};
    }

    BindGroupLayoutHandle GraphicsAllocator::CreateBindGroupLayout(
        const BindGroupLayoutCreateDesc& bindGroupLayoutCreateDesc) const {
        return mBindGroupLayoutAllocator->Allocate(mRenderDevice.GetRHI()->CreateBindGroupLayout(Graphics::Structs::BindGroupLayoutCreateDesc {
            .bindings = {bindGroupLayoutCreateDesc.bindings}
        }));
    }

    BindGroupHandle GraphicsAllocator::CreateBindGroup(const BindGroupCreateDesc& bindGroupCreateDesc) const {
        std::vector<Graphics::Structs::BindGroupBinding> bindings;
        for (const auto& binding : bindGroupCreateDesc.bindings) {
            Graphics::Structs::BindGroupBinding bind = {
                .binding = binding.binding
            };

            std::variant<Graphics::Vulkan::ImageView*, Graphics::Vulkan::Buffer*> resource;
            if (std::holds_alternative<BufferHandle>(binding.resource)) {
                bind.resource = mBufferAllocator->Get(std::get<BufferHandle>(binding.resource));
            } else {
                bind.resource = mImageViewAllocator->Get(std::get<ImageViewHandle>(binding.resource));
            }

            bindings.push_back(bind);
        }

        const auto bindGroup = mBindGroupAllocator->Allocate(mRenderDevice.GetRHI()->CreateBindGroup(Graphics::Structs::BindGroupCreateDesc {
            .layout = mBindGroupLayoutAllocator->Get(bindGroupCreateDesc.bindGroupLayout),
            .bindings = bindings
        }));

        return bindGroup;
    }

    PipelineLayoutHandle GraphicsAllocator::CreatePipelineLayout(
        const PipelineLayoutCreateDesc& pipelineLayoutCreateDesc) const {
        std::vector<Graphics::Vulkan::BindGroupLayout*> groupLayouts;
        for (const auto& layout : pipelineLayoutCreateDesc.groupLayouts) {
            groupLayouts.push_back(mBindGroupLayoutAllocator->Get(layout));
        }

        auto pipelineLayout = mPipelineLayoutAllocator->Allocate(mRenderDevice.GetRHI()->CreatePipelineLayout(Graphics::Structs::PipelineLayoutCreateDesc {
            .groupLayouts = groupLayouts
        }));

        return pipelineLayout;
    }

    GraphicsPipelineHandle GraphicsAllocator::CreateGraphicsPipeline(
        const GraphicsPipelineCreateDesc& graphicsPipelineCreateDesc) const {
        return mGraphicsPipelineAllocator->Allocate(mRenderDevice.GetRHI()->CreateGraphicsPipeline(Graphics::Structs::GraphicsPipelineCreateDesc {
            .vertexStage = mShaderAllocator->Get(graphicsPipelineCreateDesc.vertexShader),
            .pixelStage = mShaderAllocator->Get(graphicsPipelineCreateDesc.pixelShader),
            .bindings = {graphicsPipelineCreateDesc.binding},
            .colorFormats = {graphicsPipelineCreateDesc.colorFormat},
            .depthFormat = graphicsPipelineCreateDesc.depthFormat,
            .stencilFormat = graphicsPipelineCreateDesc.depthFormat,
            .pipelineLayout = mPipelineLayoutAllocator->Get(graphicsPipelineCreateDesc.pipelineLayout),
            .depthCompare = graphicsPipelineCreateDesc.depthCompare
        }));
    }

    ShaderHandle GraphicsAllocator::CreateShader(const ShaderCreateDesc& shaderCreateDesc) const {
        return mShaderAllocator->Allocate(mRenderDevice.GetRHI()->CreateShader(Graphics::Structs::ShaderCreateDesc {
            .path = shaderCreateDesc.path
        }));
    }

    void GraphicsAllocator::DestroyBuffer(BufferHandle& bufferHandle) const {
        mBufferAllocator->Free(bufferHandle);
        bufferHandle = mBufferAllocator->CreateInvalidHandle();
    }

    void GraphicsAllocator::DestroyImage(ImageHandle& imageHandle) const {
        mImageAllocator->Free(imageHandle);
        imageHandle = mImageAllocator->CreateInvalidHandle();
    }

    void GraphicsAllocator::DestroyImageView(ImageViewHandle& imageViewHandle) const {
        mImageViewAllocator->Free(imageViewHandle);
        imageViewHandle = mImageViewAllocator->CreateInvalidHandle();
    }

    void GraphicsAllocator::DestroyBindGroupLayout(BindGroupLayoutHandle& bindGroupLayoutHandle) const {
        mBindGroupLayoutAllocator->Free(bindGroupLayoutHandle);
        bindGroupLayoutHandle = mBindGroupLayoutAllocator->CreateInvalidHandle();
    }

    void GraphicsAllocator::DestroyBindGroup(BindGroupHandle& bindGroupHandle) const {
        mBindGroupAllocator->Free(bindGroupHandle);
        bindGroupHandle = mBindGroupAllocator->CreateInvalidHandle();
    }

    void GraphicsAllocator::DestroyPipelineLayout(PipelineLayoutHandle& pipelineLayoutHandle) const {
        mPipelineLayoutAllocator->Free(pipelineLayoutHandle);
        pipelineLayoutHandle = mPipelineLayoutAllocator->CreateInvalidHandle();
    }

    void GraphicsAllocator::DestroyGraphicsPipeline(GraphicsPipelineHandle& graphicsPipelineHandle) const {
        mGraphicsPipelineAllocator->Free(graphicsPipelineHandle);
        graphicsPipelineHandle = mGraphicsPipelineAllocator->CreateInvalidHandle();
    }

    void GraphicsAllocator::DestroyShader(ShaderHandle& shaderHandle) const {
        mShaderAllocator->Free(shaderHandle);
        shaderHandle = mShaderAllocator->CreateInvalidHandle();
    }
}