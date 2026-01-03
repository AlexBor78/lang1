#include <memory>
#include <format>
#include <string>

// syntax
#include <common/streams/istream.h>
#include <lang/syntax/lexer.h>
#include <lang/syntax/parser.h>

// to throw errors
#include <common/diagnostic/diagnostic.h>

#include <lang/pipeline/syntax_driver.h>


namespace lang::pipeline {
    syntax::SyntaxContainer SyntaxDriver::process_file(const std::string& file_path) {
        // check if module already processed
        if(compile_state->processed_files.contains(file_path)) throw common::diagnostic::InterError(std::format("file {} overloading", file_path));

        // tring to open file
        std::unique_ptr<common::streams::FileIStream> file = std::make_unique<common::streams::FileIStream>(file_path);
        if(!file->is_open()) throw common::diagnostic::InterError(std::format("Can not open file: {}", file_path));

        // syntax
        syntax::lexer::Lexer lexer(file.get());
        auto tokens = lexer.tokenize();

        syntax::parser::Parser parser;
        return parser.parse(tokens);
    }
}
