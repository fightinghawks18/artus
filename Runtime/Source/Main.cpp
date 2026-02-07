#include "Artus/Graphics/Surface.h"

#include <Artus/Core/Window.h>
#include <Artus/Graphics/Device.h>
#include <Artus/Graphics/Resources/CommandAllocator.h>
#include <Artus/Graphics/Resources/GraphicsPipeline.h>

using namespace Artus;

struct Vertex {
    std::array<float, 2> position;
    std::array<float, 2> uvCoords;
    std::array<float, 4> color;
};

static std::vector<Vertex> vertices = {
    {{0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{0.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
};

static std::vector<uint32_t> indices = {0, 1, 2};

int main() {
    auto* window = new Core::Window();
    auto* device = new Graphics::Device();
    auto* surface = new Graphics::Surface(*device, window);
    auto* cmdAllocator = new Graphics::CommandAllocator(*device);
    auto cmdEncoders = cmdAllocator->NewEncoders(3);

    auto* vertexBuffer = new Graphics::Buffer(*device, sizeof(Vertex) * vertices.size(), Graphics::BufferUsage::Vertex);
    auto* indexBuffer = new Graphics::Buffer(*device, sizeof(uint32_t) * indices.size(), Graphics::BufferUsage::Index);

    vertexBuffer->Map(vertices.size() * sizeof(Vertex), 0, vertices.data());
    indexBuffer->Map(indices.size() * sizeof(uint32_t), 0, indices.data());

    auto* vertexShader = new Graphics::Shader(*device, "Shaders/VS_Default.spv");
    auto* pixelShader = new Graphics::Shader(*device, "Shaders/PS_Default.spv");

    auto* pipelineLayout = new Graphics::PipelineLayout(*device);

    Graphics::GraphicsPipelineInputBinding vertexBinding = {.slot = 0, .stride = sizeof(Vertex)};
    vertexBinding.attributes.push_back(
        {.location = 0, .format = vk::Format::eR32G32B32Sfloat, .offset = offsetof(Vertex, position)});
    vertexBinding.attributes.push_back(
        {.location = 1, .format = vk::Format::eR32G32Sfloat, .offset = offsetof(Vertex, uvCoords)});
    vertexBinding.attributes.push_back(
        {.location = 2, .format = vk::Format::eR32G32B32A32Sfloat, .offset = offsetof(Vertex, color)});

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
        clearColor.setFloat32({0.2f, 0.6f, 0.3f, 1.0f});

        vk::RenderingAttachmentInfo colorAttachment = {};
        colorAttachment.setClearValue(clearColor)
            .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
            .setImageView(surface->GetVulkanImageView(imageIndex))
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eStore);

        vk::RenderingInfo renderingInfo = {};
        renderingInfo.setColorAttachments(colorAttachment).setLayerCount(1).setRenderArea(rect);

        const auto frameIndex = surface->GetFrameIndex();

        vk::Viewport viewport = {};
        viewport.setX(0)
            .setY(0)
            .setWidth(rect.extent.width)
            .setHeight(rect.extent.height)
            .setMinDepth(0)
            .setMaxDepth(1.0f);

        std::vector<vk::Rect2D> rects = {rect};
        std::vector<vk::Viewport> viewports = {viewport};

        auto cmd = cmdEncoders[frameIndex];
        cmd->Start();

        image->MakeRenderable(cmd->GetVulkanCommandBuffer());

        cmd->StartRendering(renderingInfo);

        cmd->GetVulkanCommandBuffer().setCullMode(vk::CullModeFlagBits::eNone);
        cmd->GetVulkanCommandBuffer().setDepthWriteEnable(false);
        cmd->GetVulkanCommandBuffer().setDepthTestEnable(false);
        cmd->GetVulkanCommandBuffer().setStencilTestEnable(false);
        cmd->GetVulkanCommandBuffer().setViewportWithCount(viewports.size(), viewports.data());
        cmd->GetVulkanCommandBuffer().setScissorWithCount(rects.size(), rects.data());

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
    delete vertexBuffer;
    delete indexBuffer;

    delete cmdAllocator;

    delete surface;
    delete window;
    delete device;
}
