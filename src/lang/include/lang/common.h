#pragma once

#include <cstddef>
#include <string>

namespace lang
{
    inline void debug_break() {
        #ifdef DEBUGGING
            #if defined(__x86_64__) || defined(__i386__)
                asm volatile ("int3");  // x86/x64
                
            #elif defined(__aarch64__)
                asm volatile ("brk #0");  // ARM64
            #else
            #warning "Manual breakpoint not supported on this architecture"
            #endif
        #endif
    }

    struct FileLocation {
        size_t index{0};
        size_t line{0};
        size_t column{0};
    };

    struct SourceLocation {
        std::string path;
        FileLocation start;
        FileLocation end;
        size_t length{1};
    };
}
