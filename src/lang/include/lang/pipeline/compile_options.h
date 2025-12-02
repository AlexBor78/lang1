#pragma once

#include <string>
#include <vector>

namespace lang::pipeline
{
    struct CompileOptions {
        std::string name{"main"}; // todo: rename to program_name/output_name
        std::vector<std::string> import_paths;
        // int threads{1}; // todo: multi-threading

        // features
        bool help{false};
        bool syntax_only{false};
        bool semantic_only{false};

        // debug flags
        bool trace_lexer{false};
        bool trace_parser{false};
        // bool print_tokens{false};
        // bool print_ast{false};
    };
}
