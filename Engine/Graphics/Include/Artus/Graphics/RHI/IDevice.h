//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_IDEVICE_H
#define ARTUS_IDEVICE_H

#include "Resources/IBuffer.h"
#include "Resources/IGraphicsPipeline.h"
#include "Resources/IImage.h"
#include "Resources/IImageView.h"
#include "Resources/ICommandAllocator.h"
#include "Resources/IBindGroup.h"

namespace Artus::Graphics::RHI {
    class IDevice {
    public:
        virtual ~IDevice() = 0;

        virtual IBuffer* CreateBuffer(const BufferDesc& bufferDesc) = 0;
        virtual IImage* CreateImage(const ImageDesc& imageDesc) = 0;
        virtual IImageView* CreateImageView(const ImageViewDesc& imageViewDesc) = 0;
        virtual IGraphicsPipeline* CreateGraphicsPipeline(const GraphicsPipelineDesc& pipelineDesc) = 0;
        virtual IShader* CreateShader(const ShaderDesc& shaderDesc) = 0;
        virtual IPipelineLayout* CreatePipelineLayout(const PipelineLayoutDesc& pipelineLayout) = 0;
        virtual ICommandAllocator* CreateCommandAllocator() = 0;
        virtual IBindGroup* CreateBindGroup() = 0; // TODO: Create BindGroups and BindGroupDesc
    };
}

#endif // ARTUS_IDEVICE_H