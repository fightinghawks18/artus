//
// Created by fightinghawks18 on 2/13/2026.
//


#ifndef ARTUS_RENDER_CONTEXT_H
#define ARTUS_RENDER_CONTEXT_H

#include <Artus/Graphics/Vulkan/Surface.h>
#include <Artus/Graphics/Vulkan/Device.h>

#include "Utils/ResourceAllocator.h"

#include <optional>

namespace Artus::Rendering {
    class RenderDevice;

    enum class RenderPassLoadStoreOperation {
        LoadAndStore,
        ClearAndStore
    };

    struct RenderPassDesc {
        std::vector<Handle<Graphics::Vulkan::ImageView>> colorImages;
        std::optional<Handle<Graphics::Vulkan::ImageView>> depthImage;
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