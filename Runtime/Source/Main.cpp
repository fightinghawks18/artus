#include "Artus/Graphics/Types/Geometry.h"

#include <Artus/Graphics/Surface.h>
#include <Artus/Math/Matrix4.h>
#include <Artus/Math/Vector3.h>
#include <Artus/Math/Utilities.h>

#include <Artus/Core/Window.h>
#include <Artus/Graphics/Device.h>
#include <Artus/Graphics/Resources/CommandAllocator.h>
#include <Artus/Graphics/Resources/GraphicsPipeline.h>
#include <Artus/Graphics/Resources/DescriptorAllocator.h>

using namespace Artus;

static std::vector<Graphics::Vertex3D> vertices = {
    {{ 1.0f,  0.0f,  0.5f}, {0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{ 0.5f,  0.866f,  0.5f}, {0.75f, 0.25f}, {1.0f, 0.5f, 0.0f, 1.0f}},
    {{-0.5f,  0.866f,  0.5f}, {1.0f, 0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}},
    {{-1.0f,  0.0f,  0.5f}, {0.75f, 0.75f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{-0.5f, -0.866f,  0.5f}, {0.5f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}},
    {{ 0.5f, -0.866f,  0.5f}, {0.25f, 0.75f}, {0.0f, 0.0f, 1.0f, 1.0f}},

    {{ 1.0f,  0.0f, -0.5f}, {0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{ 0.5f,  0.866f, -0.5f}, {0.75f, 0.25f}, {1.0f, 0.5f, 0.0f, 1.0f}},
    {{-0.5f,  0.866f, -0.5f}, {1.0f, 0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}},
    {{-1.0f,  0.0f, -0.5f}, {0.75f, 0.75f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{-0.5f, -0.866f, -0.5f}, {0.5f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}},
    {{ 0.5f, -0.866f, -0.5f}, {0.25f, 0.75f}, {0.0f, 0.0f, 1.0f, 1.0f}},
};

static std::vector<uint32_t> indices = {
    0, 1, 2,  0, 2, 3,  0, 3, 4,  0, 4, 5,
    6, 8, 7,  6, 9, 8,  6, 10, 9,  6, 11, 10,
    0, 6, 1,  1, 6, 7,
    1, 7, 2,  2, 7, 8,
    2, 8, 3,  3, 8, 9,
    3, 9, 4,  4, 9, 10,
    4, 10, 5,  5, 10, 11,
    5, 11, 0,  0, 11, 6,
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

    auto* vertexBuffer = new Graphics::Buffer(*device, sizeof(Graphics::Vertex3D) * vertices.size(), Graphics::BufferUsage::Vertex);
    auto* indexBuffer = new Graphics::Buffer(*device, sizeof(uint32_t) * indices.size(), Graphics::BufferUsage::Index);

    vertexBuffer->Map(vertices.size() * sizeof(Graphics::Vertex3D), 0, vertices.data());
    indexBuffer->Map(indices.size() * sizeof(uint32_t), 0, indices.data());

    auto* cameraBuffer = new Graphics::Buffer(*device, sizeof(Graphics::CameraData), Graphics::BufferUsage::Shader);

    auto* vertexShader = new Graphics::Shader(*device, "Shaders/VS_Default.spv");
    auto* pixelShader = new Graphics::Shader(*device, "Shaders/PS_Default.spv");

    std::vector<Graphics::DescriptorSetLayoutBinding> bindings;
    bindings.push_back({.binding = 0, .type = vk::DescriptorType::eUniformBuffer,
                        .stageFlags = vk::ShaderStageFlagBits::eVertex, .count = 1});

    std::vector<Graphics::DescriptorAllocatorPoolDesc> pools;
    pools.push_back({.type = vk::DescriptorType::eUniformBuffer, .descriptorCount = 2});

    auto* descriptorAllocator = new Graphics::DescriptorAllocator(*device, {
        .maxDescriptorSets = 2,
        .pools = pools
    });

    auto* descriptorLayout = descriptorAllocator->CreateDescriptorSetLayout(bindings);

    std::vector descriptorSets = {
        descriptorAllocator->CreateDescriptorSet(descriptorLayout),
        descriptorAllocator->CreateDescriptorSet(descriptorLayout)
    };

    std::vector<Graphics::PipelinePushConstant> pushConstants;
    pushConstants.push_back({.size = sizeof(Graphics::ModelData), .offset = 0, .stageFlags = vk::ShaderStageFlagBits::eVertex});

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

    Graphics::GraphicsPipelineDescriptor graphicsPipelineDescriptor = {};
    graphicsPipelineDescriptor.bindings.push_back(vertexBinding);

    graphicsPipelineDescriptor.vertexStage = vertexShader;
    graphicsPipelineDescriptor.pixelStage = pixelShader;
    graphicsPipelineDescriptor.pipelineLayout = pipelineLayout;
    graphicsPipelineDescriptor.colorFormats = {surface->GetVulkanSurfaceFormat().format};
    graphicsPipelineDescriptor.depthFormat = vk::Format::eD32SfloatS8Uint;
    graphicsPipelineDescriptor.stencilFormat = vk::Format::eD32SfloatS8Uint;

    auto* graphicsPipeline = new Graphics::GraphicsPipeline(*device, graphicsPipelineDescriptor);

    while (!window->IsClosing()) {
        Core::Window::Update();

        vk::Semaphore waitSemaphore;
        auto imageIndex = surface->AcquireNextImage(&waitSemaphore);
        auto image = surface->GetVulkanImage(imageIndex);

        vk::Rect2D rect = {};
        rect.setExtent(surface->GetVulkanExtent()).setOffset({0, 0});

        vk::ClearColorValue clearColor = {};
        clearColor.setFloat32({1.0f, 0.2f, 0.3f, 1.0f});

        vk::RenderingAttachmentInfo colorAttachment = {};
        colorAttachment.setClearValue(clearColor)
            .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
            .setImageView(surface->GetVulkanImageView(imageIndex)->GetVulkanImageView())
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eStore);

        vk::RenderingInfo renderingInfo = {};
        renderingInfo.setColorAttachments(colorAttachment).setLayerCount(1).setRenderArea(rect);

        const auto frameIndex = surface->GetFrameIndex();

        auto surfaceSize = surface->GetVulkanExtent();

        rotation *= Math::Quaternion::FromAxisAngle(Math::Vector3{1.0f, 1.0f, 1.0f}, 0.01f);
        rotation.Normalize();

        cameraData = {
            .view = Math::Matrix4::View(Math::Vector3{0, 0, 5}, Math::Vector3{0, 0, 0}, Math::Vector3{0, 1, 0}).Transpose(),
            .projection = Math::Matrix4::Perspective(Math::AsRadians(90.0f), surfaceSize.width / surfaceSize.height, 0.1f, 100.0f).Transpose()
        };

        modelData = {
            .model = (Math::Matrix4::Translate(position.x, position.y, position.z) * Math::Matrix4::QuaternionRotation(rotation) * Math::Matrix4::Scale(scale.x, scale.y, scale.z)).Transpose()
        };

        cameraBuffer->Map(sizeof(Graphics::CameraData), 0, &cameraData);
        descriptorSets[frameIndex]->WriteDescriptorSet(0, 0, vk::DescriptorType::eUniformBuffer, cameraBuffer);

        vk::Viewport viewport = {};
        viewport.setX(0)
            .setY(0)
            .setWidth(rect.extent.width)
            .setHeight(rect.extent.height)
            .setMinDepth(0)
            .setMaxDepth(1.0f);

        auto cmd = cmdEncoders[frameIndex];
        cmd->Start();

        image->MakeRenderable(cmd->GetVulkanCommandBuffer());

        cmd->StartRendering(renderingInfo);

        cmd->UpdatePushConstant(pipelineLayout, vk::ShaderStageFlagBits::eVertex, sizeof(Graphics::ModelData), 0, &modelData);
        cmd->SetCullMode(vk::CullModeFlagBits::eFront);
        cmd->SetDepthTesting(false);
        cmd->SetStencilTesting(false);
        cmd->SetViewport(viewport);
        cmd->SetScissor(rect);
        cmd->BindDescriptorSet(descriptorSets[frameIndex], pipelineLayout, 0);
        cmd->BindVertexBuffer(vertexBuffer);
        cmd->BindIndexBuffer(indexBuffer);
        cmd->BindGraphicsPipeline(graphicsPipeline);
        cmd->DrawIndexed(indices.size(), 0);

        cmd->EndRendering();

        image->MakePresentable(cmd->GetVulkanCommandBuffer());

        cmd->End();

        surface->PresentDrawn(imageIndex, cmd->GetVulkanCommandBuffer(), waitSemaphore);
    }

    device->GetVulkanDevice().waitIdle();

    delete vertexShader;
    delete pixelShader;

    delete pipelineLayout;
    delete graphicsPipeline;
    delete cameraBuffer;
    delete descriptorAllocator;
    delete vertexBuffer;
    delete indexBuffer;

    delete cmdAllocator;

    delete surface;
    delete window;
    delete device;
}
