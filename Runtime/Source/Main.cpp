#include "Artus/Graphics/Types/Geometry.h"

#include <Artus/Graphics/Surface.h>
#include <Artus/Math/Matrix4.h>
#include <Artus/Math/Utilities.h>
#include <Artus/Math/Vector3.h>

#include <Artus/Core/Window.h>
#include <Artus/Graphics/Device.h>
#include <Artus/Graphics/Resources/CommandAllocator.h>
#include <Artus/Graphics/Resources/DescriptorAllocator.h>
#include <Artus/Graphics/Resources/GraphicsPipeline.h>

#include "Artus/Math/Color.h"

using namespace Artus;

static std::vector<Graphics::Vertex3D> vertices = {
    {{1.0f, 0.0f, 0.5f}, {0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.866f, 0.5f}, {0.75f, 0.25f}, {1.0f, 0.5f, 0.0f, 1.0f}},
    {{-0.5f, 0.866f, 0.5f}, {1.0f, 0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}},
    {{-1.0f, 0.0f, 0.5f}, {0.75f, 0.75f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{-0.5f, -0.866f, 0.5f}, {0.5f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}},
    {{0.5f, -0.866f, 0.5f}, {0.25f, 0.75f}, {0.0f, 0.0f, 1.0f, 1.0f}},

    {{1.0f, 0.0f, -0.5f}, {0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.866f, -0.5f}, {0.75f, 0.25f}, {1.0f, 0.5f, 0.0f, 1.0f}},
    {{-0.5f, 0.866f, -0.5f}, {1.0f, 0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}},
    {{-1.0f, 0.0f, -0.5f}, {0.75f, 0.75f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{-0.5f, -0.866f, -0.5f}, {0.5f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}},
    {{0.5f, -0.866f, -0.5f}, {0.25f, 0.75f}, {0.0f, 0.0f, 1.0f, 1.0f}},
};

static std::vector<uint32_t> indices = {
    0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 6, 8, 7, 6, 9, 8,  6, 10, 9, 6, 11, 10, 0, 6,  1, 1, 6,  7,
    1, 7, 2, 2, 7, 8, 2, 8, 3, 3, 8, 9, 3, 9, 4, 4, 9, 10, 4, 10, 5, 5, 10, 11, 5, 11, 0, 0, 11, 6,
};

int main() {
    Math::Vector3 position{0, 0, 0};
    Math::Quaternion rotation{0, 0, 0};
    Math::Vector3 scale{1, 1, 1};

    Graphics::ModelData modelData;
    Graphics::CameraData cameraData;

    auto* window = new Core::Window();
    auto* device = new Graphics::Device();
    auto* surface = new Graphics::Surface(*device, window);
    auto* cmdAllocator = new Graphics::CommandAllocator(*device);
    auto cmdEncoders = cmdAllocator->NewEncoders(3);

    auto* vertexBuffer =
        new Graphics::Buffer(*device, sizeof(Graphics::Vertex3D) * vertices.size(), Graphics::BufferUsage::Vertex);
    auto* indexBuffer = new Graphics::Buffer(*device, sizeof(uint32_t) * indices.size(), Graphics::BufferUsage::Index);

    vertexBuffer->Map(vertices.size() * sizeof(Graphics::Vertex3D), 0, vertices.data());
    indexBuffer->Map(indices.size() * sizeof(uint32_t), 0, indices.data());

    auto* cameraBuffer = new Graphics::Buffer(*device, sizeof(Graphics::CameraData), Graphics::BufferUsage::Shader);

    std::vector<Graphics::Image*> depthImages;
    std::vector<Graphics::ImageView*> depthImageViews;
    for (uint32_t i = 0; i < 2; i++) {
        vk::Extent3D extent(surface->GetVulkanExtent(), 1);

        Graphics::ImageDesc depthImageDesc = {
            .format = vk::Format::eD32SfloatS8Uint,
            .type = vk::ImageType::e2D,
            .extent = extent,
            .usage = vk::ImageUsageFlagBits::eDepthStencilAttachment,
            .layerCount = 1,
            .levelCount = 1,
        };

        auto* depthImage = new Graphics::Image(*device, depthImageDesc);
        depthImages.push_back(depthImage);

        Graphics::ImageViewDesc depthImageViewDesc = {.image = depthImage,
                                                      .format = vk::Format::eD32SfloatS8Uint,
                                                      .type = vk::ImageViewType::e2D,
                                                      .aspectMask = vk::ImageAspectFlagBits::eDepth |
                                                                    vk::ImageAspectFlagBits::eStencil,
                                                      .baseLayer = 0,
                                                      .layerCount = 1,
                                                      .baseLevel = 0,
                                                      .levelCount = 1};

        auto* depthImageView = new Graphics::ImageView(*device, depthImageViewDesc);
        depthImageViews.push_back(depthImageView);
    }

    auto* vertexShader = new Graphics::Shader(*device, "Shaders/DefaultMesh_vs.spv");
    auto* pixelShader = new Graphics::Shader(*device, "Shaders/DefaultMesh_ps.spv");

    auto* fullscreenShader = new Graphics::Shader(*device, "Shaders/FullscreenQuad.spv");
    auto* skyboxShader = new Graphics::Shader(*device, "Shaders/DefaultSkybox.spv");

    std::vector<Graphics::DescriptorSetLayoutBinding> bindings;
    bindings.push_back({.binding = 0,
                        .type = vk::DescriptorType::eUniformBuffer,
                        .stageFlags = vk::ShaderStageFlagBits::eVertex,
                        .count = 1});

    std::vector<Graphics::DescriptorAllocatorPoolDesc> pools;
    pools.push_back({.type = vk::DescriptorType::eUniformBuffer, .descriptorCount = 2});

    auto* descriptorAllocator = new Graphics::DescriptorAllocator(*device, {.maxDescriptorSets = 2, .pools = pools});

    auto* descriptorLayout = descriptorAllocator->CreateDescriptorSetLayout(bindings);

    std::vector descriptorSets = {descriptorAllocator->CreateDescriptorSet(descriptorLayout),
                                  descriptorAllocator->CreateDescriptorSet(descriptorLayout)};

    std::vector<Graphics::PipelinePushConstant> pushConstants;
    pushConstants.push_back(
        {.size = sizeof(Graphics::ModelData), .offset = 0, .stageFlags = vk::ShaderStageFlagBits::eVertex});

    std::vector descriptorLayouts = {descriptorLayout};

    Graphics::PipelineLayoutDesc pipelineLayoutDesc;
    pipelineLayoutDesc.layouts = descriptorLayouts;
    pipelineLayoutDesc.pushConstants = pushConstants;

    auto* pipelineLayout = new Graphics::PipelineLayout(*device, pipelineLayoutDesc);

    Graphics::GraphicsPipelineInputBinding vertexBinding = {.slot = 0, .stride = sizeof(Graphics::Vertex3D)};
    vertexBinding.attributes.push_back(
        {.location = 0, .format = vk::Format::eR32G32B32Sfloat, .offset = offsetof(Graphics::Vertex3D, position)});
    vertexBinding.attributes.push_back(
        {.location = 1, .format = vk::Format::eR32G32Sfloat, .offset = offsetof(Graphics::Vertex3D, uvCoords)});
    vertexBinding.attributes.push_back(
        {.location = 2, .format = vk::Format::eR32G32B32A32Sfloat, .offset = offsetof(Graphics::Vertex3D, color)});

    Graphics::GraphicsPipelineDesc graphicsPipelineDescriptor = {};
    graphicsPipelineDescriptor.bindings.push_back(vertexBinding);

    graphicsPipelineDescriptor.vertexStage = vertexShader;
    graphicsPipelineDescriptor.pixelStage = pixelShader;
    graphicsPipelineDescriptor.pipelineLayout = pipelineLayout;
    graphicsPipelineDescriptor.colorFormats = {surface->GetVulkanSurfaceFormat().format};
    graphicsPipelineDescriptor.depthFormat = vk::Format::eD32SfloatS8Uint;
    graphicsPipelineDescriptor.stencilFormat = vk::Format::eD32SfloatS8Uint;
    graphicsPipelineDescriptor.depthCompare = vk::CompareOp::eGreater;

    auto* graphicsPipeline = new Graphics::GraphicsPipeline(*device, graphicsPipelineDescriptor);

    Graphics::PipelineLayoutDesc skyboxLayoutDesc;
    auto* skyboxLayout = new Graphics::PipelineLayout(*device, pipelineLayoutDesc);

    Graphics::GraphicsPipelineDesc fullscreenPipelineDescriptor = {};
    fullscreenPipelineDescriptor.vertexStage = fullscreenShader;
    fullscreenPipelineDescriptor.pixelStage = skyboxShader;
    fullscreenPipelineDescriptor.pipelineLayout = skyboxLayout;
    fullscreenPipelineDescriptor.colorFormats = {surface->GetVulkanSurfaceFormat().format};
    fullscreenPipelineDescriptor.depthFormat = vk::Format::eD32SfloatS8Uint;
    fullscreenPipelineDescriptor.stencilFormat = vk::Format::eD32SfloatS8Uint;
    fullscreenPipelineDescriptor.depthCompare = vk::CompareOp::eGreater;

    auto* skyboxPipeline = new Graphics::GraphicsPipeline(*device, fullscreenPipelineDescriptor);

    while (!window->IsClosing()) {
        Core::Window::Update();

        vk::Semaphore waitSemaphore;
        auto imageIndex = surface->AcquireNextImage(&waitSemaphore);
        auto image = surface->GetVulkanImage(imageIndex);
        auto depthImage = depthImages[imageIndex];

        auto surfaceRect = surface->GetRectangle();

        Graphics::RenderingAttachment surfaceColorAttachment = {
            .view = surface->GetVulkanImageView(imageIndex),
            .type = Graphics::RenderingAttachmentType::Color,
            .lsOp = Graphics::RenderingAttachmentLoadStoreOp::ClearThenStore,
            .clear = Math::Color{0.0f, 0.0f, 0.0f, 1.0f}
        };

        Graphics::RenderingAttachment surfaceDepthAttachment = {
            .view = depthImageViews[imageIndex],
            .type = Graphics::RenderingAttachmentType::DepthStencil,
            .lsOp = Graphics::RenderingAttachmentLoadStoreOp::ClearThenStore,
            .clear = Graphics::RenderingDepthStencilClear{0.0f, 0}
        };

        Graphics::RenderingPass mainPass = {
            .attachments = {surfaceColorAttachment, surfaceDepthAttachment},
            .renderArea = surfaceRect
        };

        const auto frameIndex = surface->GetFrameIndex();

        rotation *= Math::Quaternion::FromAxisAngle(Math::Vector3{1.0f, 1.0f, 1.0f}, 0.01f);
        rotation.Normalize();

        cameraData = {
            .view =
                Math::Matrix4::View(Math::Vector3{0, 0, 2}, Math::Vector3{0, 0, 0}, Math::Vector3{0, 1, 0}).Transpose(),
            .projection =
                Math::Matrix4::Perspective(Math::AsRadians(90.0f), surfaceRect.width / surfaceRect.height, 0.1f, 100.0f)
                    .Transpose()};

        modelData = {.model =
                         (Math::Matrix4::Translate(position.x, position.y, position.z) *
                          Math::Matrix4::QuaternionRotation(rotation) * Math::Matrix4::Scale(scale.x, scale.y, scale.z))
                             .Transpose()};

        cameraBuffer->Map(sizeof(Graphics::CameraData), 0, &cameraData);
        descriptorSets[frameIndex]->WriteDescriptorSet(0, 0, vk::DescriptorType::eUniformBuffer, cameraBuffer);

        auto cmd = cmdEncoders[frameIndex];
        cmd->Start();

        cmd->MakeImageRenderable(image);
        cmd->MakeImageDepthStencil(depthImage);

        cmd->StartRenderingPass(mainPass);

        cmd->SetViewport({0, 0, static_cast<float>(surfaceRect.width), static_cast<float>(surfaceRect.height), 0.0f, 1.0f});
        cmd->SetScissor(surfaceRect);

        cmd->SetDepthTesting(false);
        cmd->SetDepthWriting(false);
        cmd->SetStencilTesting(false);
        cmd->BindGraphicsPipeline(skyboxPipeline);
        cmd->SetCullMode(vk::CullModeFlagBits::eNone);
        cmd->Draw(3, 0);

        cmd->UpdatePushConstant(pipelineLayout, vk::ShaderStageFlagBits::eVertex, sizeof(Graphics::ModelData), 0,
                                &modelData);
        cmd->SetCullMode(vk::CullModeFlagBits::eFront);
        cmd->SetDepthTesting(true);
        cmd->SetDepthWriting(true);
        cmd->SetStencilTesting(false);
        cmd->BindDescriptorSet(descriptorSets[frameIndex], pipelineLayout);
        cmd->BindVertexBuffer(vertexBuffer);
        cmd->BindIndexBuffer(indexBuffer);
        cmd->BindGraphicsPipeline(graphicsPipeline);
        cmd->DrawIndexed(indices.size(), 0);

        cmd->EndRenderingPass();

        cmd->MakeImagePresentable(image);

        cmd->End();

        surface->PresentDrawn(imageIndex, cmd->GetVulkanCommandBuffer(), waitSemaphore);
    }

    device->GetVulkanDevice().waitIdle();

    for (auto& view : depthImageViews) {
        delete view;
    }

    for (auto& image : depthImages) {
        delete image;
    }

    delete vertexShader;
    delete pixelShader;

    delete pipelineLayout;
    delete graphicsPipeline;
    delete skyboxPipeline;
    delete skyboxLayout;
    delete fullscreenShader;
    delete skyboxShader;
    delete cameraBuffer;
    delete descriptorAllocator;
    delete vertexBuffer;
    delete indexBuffer;

    delete cmdAllocator;

    delete surface;
    delete window;
    delete device;
}
