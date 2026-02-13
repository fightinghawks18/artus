//
// Created by fightinghawks18 on 2/12/26.
//


#ifndef ARTUS_IBIND_GROUP_LAYOUT_H
#define ARTUS_IBIND_GROUP_LAYOUT_H

#include "../Types/Common.h"

#include <vector>
#include <variant>

#include "IBuffer.h"
#include "IImageView.h"

namespace Artus::Graphics::RHI {
    enum class BindGroupLayoutBindingType {
        Buffer,
        ImageView
    };

    struct BindGroupLayoutBinding {
        ShaderStage visibility;
        uint32_t binding;
        BindGroupLayoutBindingType type;
    };

    struct BindGroupLayoutCreateDesc {
        std::vector<BindGroupLayoutBinding> bindings;
    };

    struct BindGroupLayoutDesc : BindGroupLayoutCreateDesc {};

    class IBindGroupLayout {
    public:
        virtual ~IBindGroupLayout() = default;

        [[nodiscard]] virtual const BindGroupLayoutDesc& GetDesc() const = 0;
    };
}

#endif // ARTUS_IBIND_GROUP_LAYOUT_H
