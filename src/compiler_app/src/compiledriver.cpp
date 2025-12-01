#include <format>
#include <stdexcept>

#include <lang/utils/istream.h>
#include <lang/syntax/lexer.h>
#include <lang/syntax/parser.h>
#include <lang/utils/ast_utils.h>
#include <lang/utils/syntax_utils.h>

#include <compiler/compiledriver.h>

namespace compiler_app {
    
    void CompileDriver::run(CompileOptions _options) {
        options = std::move(_options);
        run();
    }

    void CompileDriver::run() {
        // open main file
        lang::utils::FileIStream file(options.main_file);
        if(!file.is_open()) {
            throw std::runtime_error(std::format("can not open file: {}", options.main_file));
        }
        
        // tokenize main file
        lang::syntax::lexer::Lexer lexer(&file);
        auto tokens = lexer.tokenize();
        if(!lexer.is_success()) {
            return; // todo: update lexer api
        }
        if(options.print_tokens) {
            lang::utils::print(tokens);
        }
        
        // build ast for main file
        lang::syntax::parser::Parser parer;
        auto ast = parer.parse(tokens);
        if(options.print_ast) {
            lang::utils::print(ast);
        }
        
        if(options.syntax_only) return;

        // semantic
        // todo:

        // ImportAnalyzer
        // ..
    
    }
}
