#pragma once

#include <string>
#include <vector>

namespace lang::pipeline
{
    /** 
     * @todo rename lname -> program_name/output_name
     * @todo multithreading
     * @todo support for trace_lexer
     * @todo support for trace_parser
     */
    struct CompileOptions {
        std::string name{"main"}; // todo: rename to program_name/output_name
        std::vector<std::string> import_paths;
        // int threads{1}; // todo: multi-threading

        // features
        bool help{false};
        bool syntax_only{false};
        bool semantic_only{false};

        // debug flags
        bool trace_lexer{false}; // todo: SyntaxDriver, Lexer support
        bool trace_parser{false}; // todo: SyntaxDriver, Parser support
        std::vector<std::string> print_modules_ast;
        std::vector<std::string> print_modules_tokens;
    };
}
