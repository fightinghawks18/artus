//
// Created by fightinghawks18 on 1/24/26.
//

#include "Artus/Graphics/Utils/Metal/Layer.h"

#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>

namespace Artus::Graphics::Metal {
    void* CreateMetalLayer(Core::Window* window) {
        NSWindow* windowHandle = (__bridge NSWindow*)window->GetHandle().handle;
        NSView* view = [windowHandle contentView];
        if (view.wantsLayer && view.layer != nil)
            return view.layer;
        
        view.wantsLayer = YES;
        view.layer = [CAMetalLayer layer];
        return (__bridge void*)view.layer;
    }
}
