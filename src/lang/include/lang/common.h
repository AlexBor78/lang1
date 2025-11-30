#pragma once

#include <cstddef>
#include <string>

// Debug flags for different components
#ifdef DEBUGGING
    // #define LEXER_DEBUG
    // #define PARSER_DEBUG
    // #define SEMANTIC_DEBUG
    // #define CODEGEN_DEBUG
#endif

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
        bool operator==(const FileLocation& other) const noexcept {
            return index  == other.index
            &&     line   == other.line
            &&     column == other.column;
        }
    };

    struct SourceLocation {
        std::string path;
        FileLocation start;
        FileLocation end;
        size_t length{1};

        bool operator==(const SourceLocation& other) const noexcept {
            return path   == other.path
            &&     start  == other.start
            &&     end    == other.end
            &&     length == other.length;
        }
    };
}
