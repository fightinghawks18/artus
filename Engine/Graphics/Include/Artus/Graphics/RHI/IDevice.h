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

        virtual ISurface* CreateSurface(const SurfaceCreateDesc& surfaceCreateDesc) = 0;
        virtual IBuffer* CreateBuffer(const BufferCreateDesc& bufferCreateDesc) = 0;
        virtual IImage* CreateImage(const ImageCreateDesc& imageCreateDesc) = 0;
        virtual IImageView* CreateImageView(const ImageViewCreateDesc& imageViewCreateDesc) = 0;
        virtual IGraphicsPipeline* CreateGraphicsPipeline(const GraphicsPipelineCreateDesc& pipelineCreateDesc) = 0;
        virtual IShader* CreateShader(const ShaderCreateDesc& shaderCreateDesc) = 0;
        virtual IPipelineLayout* CreatePipelineLayout(const PipelineLayoutCreateDesc& pipelineLayout) = 0;
        virtual ICommandEncoder* CreateCommandEncoder() = 0;
        virtual std::vector<ICommandEncoder*> CreateCommandEncoders(uint32_t commandEncoderCount) = 0;
        virtual IBindGroupLayout* CreateBindGroupLayout(const BindGroupLayoutCreateDesc& bindGroupLayoutCreateDesc) = 0;
        virtual IBindGroup* CreateBindGroup(const BindGroupCreateDesc& bindGroupDesc) = 0;
    };
}

#endif // ARTUS_IDEVICE_H
