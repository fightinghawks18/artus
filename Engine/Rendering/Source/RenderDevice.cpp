//
// Created by fightinghawks18 on 2/13/2026.
//

#include "Artus/Rendering/RenderDevice.h"

#include "Artus/Core/Logger.h"
#include "Artus/Graphics/Vulkan/Resources/BindGroupLayout.h"

#include <Artus/Graphics/Vulkan/Device.h>

namespace Artus::Rendering {
    RenderDevice::RenderDevice(const RenderDeviceCreateDesc& desc) : mRenderingApi(desc.renderingApi), mMainWindow(desc.mainWindow) {
        CreateRenderDevice();
        CreateAllocators();
        CreateMainSurface();
        CreateResources();
        CreateSurfaceResources();
        mSurfaceReady = true;
    }

    RenderDevice::~RenderDevice() {
        DestroyResources();
        DestroySurfaceResources();
        DestroyMainSurface();
        DestroyAllocators();
        DestroyRenderDevice();
    }

    MaterialGroupHandle RenderDevice::CreateMaterialGroup(const MaterialGroupCreateDesc& materialGroupCreateDesc) const {
        const auto materialGroup = new MaterialGroup();
        materialGroup->pipeline = mGraphicsAllocator->CreateGraphicsPipeline(materialGroupCreateDesc.pipelineCreateDesc);
        materialGroup->pipelineLayout = mGraphicsAllocator->CreatePipelineLayout(materialGroupCreateDesc.pipelineLayoutCreateDesc);
        for (const auto& groupLayout : materialGroupCreateDesc.bindGroupLayoutCreateDescs) {
            materialGroup->bindGroupLayouts.push_back(mGraphicsAllocator->CreateBindGroupLayout(groupLayout));
        }

        return mMaterialGroupAllocator->Allocate(materialGroup);
    }

    MaterialHandle RenderDevice::CreateMaterial(const MaterialCreateDesc& materialCreateDesc) {
        ImageCreateDesc diffuseDesc = {
            .width = 800,
            .height = 600,
            .format = mSurface->GetFormat(),
            .usage = Graphics::Flags::ImageUsage::Color
        };

        const auto material = new Material();
        material->materialGroup = materialCreateDesc.materialGroup;
        material->diffuse = mGraphicsAllocator->CreateImage(diffuseDesc);

        auto materialGroup = mMaterialGroupAllocator->Get(material->materialGroup);

        for (uint32_t i = 0; i < materialGroup->bindGroupLayouts.size(); i++) {
            BindGroupCreateDesc bindGroupDesc = {
                .bindGroupLayout = materialGroup->bindGroupLayouts[i],
                .bindings = {}
            };

            for (const auto& createDesc : materialGroup->layoutCreateDescs[i].bindings) {
                std::variant<ImageViewHandle, BufferHandle> resource;
                switch (createDesc.type) {
                case Graphics::Enums::BindGroupLayoutBindingType::Buffer:
                    break;
                case Graphics::Enums::BindGroupLayoutBindingType::ImageView:
                    break;
                }

                bindGroupDesc.bindings.push_back(BindGroupBinding{
                    .binding = createDesc.binding,
                    .resource = nullptr
                });
            }

            material->bindGroups.push_back(mGraphicsAllocator->CreateBindGroup(bindGroupDesc));
        }

        return mMaterialAllocator->Allocate(material);
    }

    RenderState RenderDevice::StartRendering() {
        if (!mSurfaceReady)
            return RenderState(RenderStartResult::Fail, RenderContext(*this));

        mSurface->PrepareFrame();
        mImageIndex = mSurface->GetImageIndex();

        const auto currentEncoder = GetCurrentCommandEncoder();
        auto currentSurfaceColorImageHandle = GetCurrentSurfaceColorImage();
        const auto surfaceColorImage = GetImage(currentSurfaceColorImageHandle.img);

        auto currentSurfaceDepthImageHandle = GetCurrentSurfaceDepthImage();
        const auto surfaceDepthImage = GetImage(currentSurfaceDepthImageHandle.img);

        currentEncoder->Start();
        currentEncoder->MakeImageRenderable(surfaceColorImage);
        currentEncoder->MakeImageDepthStencil(surfaceDepthImage);
        return RenderState(RenderStartResult::Ok, RenderContext(*this));
    }

    void RenderDevice::EndRendering(RenderState& state) {
        const auto currentEncoder = GetCurrentCommandEncoder();
        auto currentSurfaceColorImageHandle = GetCurrentSurfaceColorImage();
        const auto surfaceColorImage = GetImage(currentSurfaceColorImageHandle.img);

        currentEncoder->MakeImagePresentable(surfaceColorImage);
        currentEncoder->End();
        mSurface->PresentFrame(state->GetRawCmd());
        mFrameIndex = (mFrameIndex + 1) % 2;
    }

    Graphics::Vulkan::Image* RenderDevice::GetImage(Handle<Graphics::Vulkan::Image>& handle) const {
        return mGraphicsAllocator->GetImageAllocator()->Get(handle);
    }

    Graphics::Vulkan::ImageView* RenderDevice::GetImageView(Handle<Graphics::Vulkan::ImageView>& handle) const {
        return mGraphicsAllocator->GetImageViewAllocator()->Get(handle);
    }

    Graphics::Vulkan::CommandEncoder* RenderDevice::GetCurrentCommandEncoder() const {
        return mCommandEncoders[mFrameIndex].get();
    }

    Graphics::Vulkan::Surface* RenderDevice::GetMainSurface() const {
        return mSurface.get();
    }

    Graphics::Vulkan::Device* RenderDevice::GetRHI() const {
        return mRHI.get();
    }

    GraphicsAllocator* RenderDevice::GetGraphicsAllocator() const {
        return mGraphicsAllocator.get();
    }

    void RenderDevice::CreateRenderDevice() {
        if (mRenderingApi == RenderAPI::Vulkan) {
            mRHI = std::make_unique<Graphics::Vulkan::Device>();
        }
    }

    void RenderDevice::CreateAllocators() {
        mGraphicsAllocator = std::make_unique<GraphicsAllocator>(*this);
        mMaterialAllocator = std::make_unique<ResourceAllocator<Material>>();
        mMaterialGroupAllocator = std::make_unique<ResourceAllocator<MaterialGroup>>();
    }

    void RenderDevice::CreateMainSurface() {
        mSurface = std::unique_ptr<Graphics::Vulkan::Surface>(mRHI->CreateSurface({ .window = mMainWindow }));

        mSurface->OnResize([&] {
            mSurfaceReady = false;
            DestroySurfaceResources();
            CreateSurfaceResources();
            mSurfaceReady = true;
        });
    }

    void RenderDevice::CreateResources() {
        for (auto& encoder : mRHI->CreateCommandEncoders(2)) {
            mCommandEncoders.push_back(std::unique_ptr<Graphics::Vulkan::CommandEncoder>(std::move(encoder)));
        }
    }

    void RenderDevice::CreateSurfaceResources() {
        auto colorImages = mSurface->GetColorImages();
        auto colorImageViews = mSurface->GetColorImageViews();
        auto depthImages = mSurface->GetDepthImages();
        auto depthImageViews = mSurface->GetDepthImageViews();

        for (uint32_t i = 0; i < colorImages.size(); i++) {
            mSurfaceColorImages.push_back({
                .img = mGraphicsAllocator->GetImageAllocator()->AllocateNonOwning(colorImages[i]),
                .view = mGraphicsAllocator->GetImageViewAllocator()->AllocateNonOwning(colorImageViews[i])
            });

            mSurfaceDepthImages.push_back({
                .img = mGraphicsAllocator->GetImageAllocator()->AllocateNonOwning(depthImages[i]),
                .view = mGraphicsAllocator->GetImageViewAllocator()->AllocateNonOwning(depthImageViews[i])
            });
        }
    }

    void RenderDevice::DestroySurfaceResources() {
        for (uint32_t i = 0; i < mSurfaceColorImages.size(); i++) {
            mGraphicsAllocator->DestroyImage(mSurfaceColorImages[i].img);
            mGraphicsAllocator->DestroyImageView(mSurfaceColorImages[i].view);
            mGraphicsAllocator->DestroyImage(mSurfaceDepthImages[i].img);
            mGraphicsAllocator->DestroyImageView(mSurfaceDepthImages[i].view);
        }
        mSurfaceColorImages.clear();
        mSurfaceDepthImages.clear();
    }

    void RenderDevice::DestroyResources() {
        mCommandEncoders.clear();
    }

    void RenderDevice::DestroyMainSurface() {
        mSurface.reset();
    }

    void RenderDevice::DestroyAllocators() {
        mMaterialAllocator.reset();
        mMaterialGroupAllocator.reset();
        mGraphicsAllocator.reset();
    }

    void RenderDevice::DestroyRenderDevice() {
        mRHI.reset();
    }
}