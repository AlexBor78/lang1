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

namespace lang
{
    /** 
     * @todo multithreading
     * @todo support for trace_lexer
     * @todo support for trace_parser
     */
    struct CompileOptions {
        std::string output_name{"a.out"};
        std::vector<std::string> import_paths;
        std::vector<std::string> inputs_files = {};// {"main.lang"};
        // int threads{1}; // todo: multi-threading

        // features
				bool dry_run{false};
        bool syntax_only{false};
        bool semantic_only{false};

        // debug flags
        std::vector<std::string> print_modules_ast; // print ast of that modules
        std::vector<std::string> print_modules_tokens; // print tokens of that modules

				// verbose
				bool verbose_all{false};
				bool verbose_syntax{false};
				bool verbose_lexer{false};
				bool verbose_parser{false};

				// trace
        bool trace_lexer{false};
        bool trace_parser{false};

				// other options
				size_t memory_page_size{4096}; // needs for padding
				size_t ast_arena_init_size{64 * 1024}; // by defalt arena starts from 64kb
				std::string extension{".lang"};
    };
}
