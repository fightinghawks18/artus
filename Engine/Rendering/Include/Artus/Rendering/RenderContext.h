//
// Created by fightinghawks18 on 2/13/2026.
//


#ifndef ARTUS_RENDER_CONTEXT_H
#define ARTUS_RENDER_CONTEXT_H

#include "Utils/GraphicsAllocator.h"

#include <Artus/Graphics/Vulkan/Surface.h>
#include <Artus/Graphics/Vulkan/Device.h>

#include <optional>

namespace Artus::Rendering {
    class RenderDevice;

    enum class RenderPassLoadStoreOperation {
        LoadAndStore,
        ClearAndStore
    };

    struct RenderPassDesc {
        std::vector<ImageViewHandle> colorImages;
        std::optional<ImageViewHandle> depthImage;
        RenderPassLoadStoreOperation loadStoreOperation;
    };

    class RenderContext {
    public:
        explicit RenderContext(RenderDevice& renderDevice);
        ~RenderContext();

        void StartPass(const RenderPassDesc& renderPassDesc);
        void EndPass();

        [[nodiscard]] Graphics::Vulkan::CommandEncoder* GetRawCmd() const;

    private:
        RenderDevice& mRenderDevice;
    };
}

#endif // ARTUS_RENDER_CONTEXT_H