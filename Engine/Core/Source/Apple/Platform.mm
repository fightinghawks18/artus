#include "Core/Platform.h"

#import <AppKit/AppKit.h>
#import <CoreFoundation/CoreFoundation.h>

@interface WindowDelegate : NSObject<NSWindowDelegate>
@property (nonatomic) Artus::Core::Window* win;
@end

@implementation WindowDelegate
- (BOOL)windowShouldClose:(NSWindow *)window {
    self.win->SetClosing(true);
    return NO;
}
- (void)windowWillClose:(NSWindow *)window {
    [[window delegate] release];
}
@end

namespace Artus::Core {
    void Window::Update() {
        NSEvent* e;
        while ((e = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES])) {
            [NSApp sendEvent:e];
        }
    }

    struct Window::Impl {
        NSWindow* win;
    };

    Window::Window() : mImpl(std::make_unique<Impl>()) {
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        [NSApp activate];
        
        NSWindow* win = [NSWindow alloc];
        [win initWithContentRect:NSMakeRect(0, 0, 800, 600)
                       styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable
                         backing:NSBackingStoreBuffered
                           defer:NO];
        [win makeKeyAndOrderFront:nil];
        
        WindowDelegate* delegate = [WindowDelegate alloc];
        [delegate init];
        delegate.win = this;
        [win setDelegate:delegate];
        
        mImpl->win = win;
        mClosing = false;
    }

    Window::~Window() {
        if (mImpl->win == nil)
            return;
        [mImpl->win release];
        mImpl->win = nil;
    }
}
