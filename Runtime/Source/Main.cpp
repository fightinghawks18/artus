//
// Created by fightinghawks18 on 2/12/2026
//

#include <Artus/Graphics/RHI/IDevice.h>
#include <Artus/Graphics/RHI/Types/Geometry.h>
#include <Artus/Graphics/Vulkan/Device.h>
#include <Artus/Core/Window.h>
#include <Artus/Math/Utilities.h>

using namespace Artus;

static std::vector<Graphics::RHI::Vertex3D> vertices = {
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
    Graphics::RHI::CameraData camera = {
        .view = Math::Matrix4::View(Math::Vector3{0, 0.5, 2}, Math::Vector3{0, 0, 0}, Math::Vector3::Up()).Transpose(),
        .projection = Math::Matrix4::Perspective(Math::AsRadians(90.0f), 1920 / 1080, 0.1f, 100.0f).Transpose()
    };

    Graphics::RHI::ModelData model0 = {
        .model = (Math::Matrix4::Translate(0, 0, 0) *
            (Math::Matrix4::RotateX(0) * Math::Matrix4::RotateY(0) * Math::Matrix4::RotateZ(0)) *
                Math::Matrix4::Scale(1, 1, 1)).Transpose()
    };

    Graphics::RHI::ModelData model1 = {
        .model = (Math::Matrix4::Translate(0.15, -0.5, 0) *
            (Math::Matrix4::RotateX(0) * Math::Matrix4::RotateY(Math::AsRadians(50.0f)) * Math::Matrix4::RotateZ(0)) *
                Math::Matrix4::Scale(1, 1, 1)).Transpose()
    };


    auto* window = new Core::Window();
    Graphics::RHI::IDevice* device = new Graphics::Vulkan::Device();

    auto* surface = device->CreateSurface({
        .window = window
    });

    const auto cmds = device->CreateCommandEncoders(3);

    auto* vertexBuffer = device->CreateBuffer({
        .size = sizeof(Graphics::RHI::Vertex3D) * vertices.size(),
        .usage = Graphics::RHI::BufferUsage::Vertex
    });

    auto* indexBuffer = device->CreateBuffer({
        .size = sizeof(uint32_t) * indices.size(),
        .usage = Graphics::RHI::BufferUsage::Index
    });

    vertexBuffer->Map<Graphics::RHI::Vertex3D>(vertices);
    indexBuffer->Map<uint32_t>(indices);

    auto* objectVertexShader = device->CreateShader({ .path = "Shaders/DefaultMesh_vs.spv" });
    auto* objectPixelShader = device->CreateShader({ .path = "Shaders/DefaultMesh_ps.spv" });

    auto* cameraBuffer = device->CreateBuffer({
        .size = sizeof(Graphics::RHI::CameraData),
        .usage = Graphics::RHI::BufferUsage::Shader
    });

    auto* objectBuffer = device->CreateBuffer({
        .size = sizeof(Graphics::RHI::ModelData),
        .usage = Graphics::RHI::BufferUsage::Shader
    });

    auto* object1Buffer = device->CreateBuffer({
        .size = sizeof(Graphics::RHI::ModelData),
        .usage = Graphics::RHI::BufferUsage::Shader
    });

    cameraBuffer->Map(sizeof(Graphics::RHI::CameraData), 0, &camera);
    objectBuffer->Map(sizeof(Graphics::RHI::ModelData), 0, &model0);
    object1Buffer->Map(sizeof(Graphics::RHI::ModelData), 0, &model1);

    Graphics::RHI::BindGroupLayoutBinding cameraBinding = {
        .visibility = Graphics::RHI::ShaderStage::Vertex,
        .binding = 0,
        .type = Graphics::RHI::BindGroupLayoutBindingType::Buffer
    };

    auto* cameraGroupLayout = device->CreateBindGroupLayout({
        .bindings = {cameraBinding}
    });

    Graphics::RHI::BindGroupBinding cameraBufferBinding = {
        .binding = 0,
        .resource = cameraBuffer
    };

    auto* cameraGroup = device->CreateBindGroup({ .layout = cameraGroupLayout, .bindings = {cameraBufferBinding} });

    Graphics::RHI::BindGroupLayoutBinding objectBinding = {
        .visibility = Graphics::RHI::ShaderStage::Vertex,
        .binding = 0,
        .type = Graphics::RHI::BindGroupLayoutBindingType::Buffer
    };

    auto* objectGroupLayout = device->CreateBindGroupLayout({
        .bindings = {objectBinding}
    });

    Graphics::RHI::BindGroupBinding objectModel0Binding = {
        .binding = 0,
        .resource = objectBuffer
    };

    auto* objectGroup = device->CreateBindGroup({ .layout = objectGroupLayout, .bindings = {objectModel0Binding} });

    auto* objectPipelineLayout = device->CreatePipelineLayout({
        .groupLayouts = {cameraGroupLayout, objectGroupLayout}
    });


    Graphics::RHI::BindGroupBinding objectModel1Binding = {
        .binding = 0,
        .resource = object1Buffer
    };

    auto* object1Group = device->CreateBindGroup({ .layout = objectGroupLayout, .bindings = {objectModel1Binding} });

    Graphics::RHI::GraphicsPipelineInputAttribute position = {
        .location = 0,
        .format = Graphics::RHI::Format::RGB32_Float,
        .offset = offsetof(Graphics::RHI::Vertex3D, position)
    };

    Graphics::RHI::GraphicsPipelineInputAttribute uv = {
        .location = 1,
        .format = Graphics::RHI::Format::RG32_Float,
        .offset = offsetof(Graphics::RHI::Vertex3D, uvCoords)
    };

    Graphics::RHI::GraphicsPipelineInputAttribute color = {
        .location = 2,
        .format = Graphics::RHI::Format::RGBA32_Float,
        .offset = offsetof(Graphics::RHI::Vertex3D, color)
    };

    Graphics::RHI::GraphicsPipelineInputBinding vertex = {
        .slot = 0,
        .stride = sizeof(Graphics::RHI::Vertex3D),
        .attributes = {position, uv, color}
    };

    auto* objectGraphicsPipeline = device->CreateGraphicsPipeline({
        .vertexStage = objectVertexShader,
        .pixelStage = objectPixelShader,
        .bindings = {vertex},
        .colorFormats = {surface->GetFormat()},
        .depthFormat = Graphics::RHI::Format::D32_SFloat_S8_UInt,
        .stencilFormat = Graphics::RHI::Format::D32_SFloat_S8_UInt,
        .pipelineLayout = objectPipelineLayout,
        .depthCompare = Graphics::RHI::DepthComparison::Greater
    });

    while (!window->IsClosing()) {
        Core::Window::Update();

        auto* cmd = cmds[surface->GetFrameIndex()];

        const auto frameInfo = surface->PrepareFrame();
        if (!frameInfo.colorImage || !frameInfo.colorView)
            continue;

        Graphics::RHI::RenderingAttachment surfaceColorAttachment = {
            .view = frameInfo.colorView,
            .type = Graphics::RHI::RenderingAttachmentType::Color,
            .lsOp = Graphics::RHI::RenderingAttachmentLoadStoreOp::ClearThenStore,
            .clear = Math::Color{0.0f, 0.0f, 0.0f, 1.0f}
        };

        Graphics::RHI::RenderingAttachment surfaceDepthAttachment = {
            .view = frameInfo.depthView,
            .type = Graphics::RHI::RenderingAttachmentType::DepthStencil,
            .lsOp = Graphics::RHI::RenderingAttachmentLoadStoreOp::ClearThenStore,
            .clear = Graphics::RHI::RenderingDepthStencilClear{0.0f, 0}
        };

        Graphics::RHI::RenderingPass renderPass = {
            .attachments = {surfaceColorAttachment, surfaceDepthAttachment},
            .renderArea = surface->GetRectangle()
        };

        auto rect = surface->GetRectangle();

        cmd->Start();
        cmd->MakeImageRenderable(frameInfo.colorImage);
        cmd->MakeImageDepthStencil(frameInfo.depthImage);
        cmd->StartRenderingPass(renderPass);

        cmd->SetViewport({0, 0, static_cast<float>(rect.width), static_cast<float>(rect.height), 0.0f, 1.0f});
        cmd->SetScissor(rect);
        cmd->SetDepthTesting(false);
        cmd->SetDepthWriting(false);
        cmd->SetCullMode(Graphics::RHI::CullMode::Front);
        cmd->SetStencilTesting(false);

        cmd->BindGraphicsPipeline(objectGraphicsPipeline);
        cmd->BindVertexBuffer(vertexBuffer);
        cmd->BindIndexBuffer(indexBuffer);
        cmd->BindGroup(0, cameraGroup, objectPipelineLayout);
        cmd->BindGroup(1, objectGroup, objectPipelineLayout);
        cmd->DrawIndexed(indices.size(), 0);

        cmd->BindGroup(0, cameraGroup, objectPipelineLayout);
        cmd->BindGroup(1, object1Group, objectPipelineLayout);
        cmd->DrawIndexed(indices.size(), 0);

        cmd->EndRenderingPass();
        cmd->MakeImagePresentable(frameInfo.colorImage);
        cmd->End();

        surface->PresentFrame(cmd);
    }
}