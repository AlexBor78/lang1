#pragma once

#include <string_view>

// #define DEBUGING

namespace lang
{
    inline void debug_break() {
        #ifdef DEBUGING
            #if defined(__x86_64__) || defined(__i386__)
                asm volatile ("int3");  // x86/x64
                
            #elif defined(__aarch64__)
                asm volatile ("brk #0");  // ARM64
            #else
            #warning "Manual breakpoint not supported on this architecture"
            #endif
        #endif
    }

    struct Position
    {
        std::string_view path;
        size_t line{0}; // unused
        size_t column{0}; // unused
        size_t start{0}; // character pos
        size_t length{1};
    };
}
