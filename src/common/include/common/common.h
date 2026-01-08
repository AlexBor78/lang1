#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <common/diagnostic/diagnostic.h>

// Debug flags for different components
#ifdef DEBUGGING
    // #define LEXER_DEBUG
    // #define PARSER_DEBUG
    // #define SEMANTIC_DEBUG
    // #define CODEGEN_DEBUG
#endif

namespace common
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

    /**
     * @brief position in file, just point
     * @todo rename to FilePosition
     */
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

    /**
     * @brief location in file, not just point - range from start to end
     * 
     */
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

        /**
         * @brief merge positions, create maximum range of both
         * 
         * @param other 
         */
        void merge(const SourceLocation& other) {
            if(path != other.path) throw diagnostic::InterError("can not merge locations from different files");
            if(start.index > other.start.index) start = other.start;
            if(end.index < other.end.index)     end   = other.end;
            assert(start.index <= end.index);
        }
    };
}
