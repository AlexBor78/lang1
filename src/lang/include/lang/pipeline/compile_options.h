#pragma once
/**
 * @file compile_options.h
 * @author alex (borproyoutob@gmail.com)
 * @brief file contains `struct CompileOptions`
 * 
 * @todo rename file to compile_type and combine with compile_state.h
 */

#include <string>
#include <vector>

namespace lang::pipeline
{
    /** 
     * @todo multithreading
     * @todo support for trace_lexer
     * @todo support for trace_parser
     */
    struct CompileOptions {
        std::string output_name{"a.out"};
        std::vector<std::string> import_paths;
        std::vector<std::string> inputs_files = {"main.lang"};
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
