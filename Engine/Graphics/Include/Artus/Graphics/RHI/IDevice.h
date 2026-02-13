//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_IDEVICE_H
#define ARTUS_IDEVICE_H

#include "ISurface.h"
#include "Resources/IBuffer.h"
#include "Resources/IGraphicsPipeline.h"
#include "Resources/IImage.h"
#include "Resources/IImageView.h"
#include "Resources/ICommandEncoder.h"
#include "Resources/IBindGroup.h"
#include "Resources/IBindGroupLayout.h"

namespace Artus::Graphics::RHI {
    class IDevice {
    public:
        virtual ~IDevice() = default;

        virtual ISurface* CreateSurface(const SurfaceDesc& surfaceDesc) = 0;
        virtual IBuffer* CreateBuffer(const BufferDesc& bufferDesc) = 0;
        virtual IImage* CreateImage(const ImageDesc& imageDesc) = 0;
        virtual IImageView* CreateImageView(const ImageViewDesc& imageViewDesc) = 0;
        virtual IGraphicsPipeline* CreateGraphicsPipeline(const GraphicsPipelineDesc& pipelineDesc) = 0;
        virtual IShader* CreateShader(const ShaderDesc& shaderDesc) = 0;
        virtual IPipelineLayout* CreatePipelineLayout(const PipelineLayoutDesc& pipelineLayout) = 0;
        virtual ICommandEncoder* CreateCommandEncoder() = 0;
        virtual std::vector<ICommandEncoder*> CreateCommandEncoders(uint32_t commandEncoderCount) = 0;
        virtual IBindGroupLayout* CreateBindGroupLayout(const BindGroupLayoutDesc& bindGroupLayoutDesc) = 0;
        virtual IBindGroup* CreateBindGroup(const BindGroupDesc& bindGroupDesc) = 0;
    };
}

#endif // ARTUS_IDEVICE_H
