#pragma once

#include <string>
#include <unordered_set>

namespace lang::pipeline {
    struct CompileState {
        /**
         * @brief list of files already processed
         */
        std::unordered_set<std::string> processed_files;
    };
}
