#include <memory>
#include <format>
#include <string>

// syntax
#include <common/streams/istream.h>
#include <lang/common/compile/source_file.h>
#include <lang/syntax/lexer.h>
#include <lang/syntax/parser.h>

// to throw errors
#include <common/diagnostic/diagnostic.h>

#include <lang/pipeline/syntax_driver.h>

namespace lang::pipeline {
    syntax::SyntaxContainer SyntaxDriver::process_file(const std::string& file_path) {
        // check if module already processed
        if(program->compile_state.processed_files.contains(file_path)) 
					throw common::diagnostic::InterError(std::format("file {} overloading", file_path));

				program->logger.set_name("SYNTAX");
				program->logger.log("processing file: {}", file_path);

        // tring to open file
        std::unique_ptr<common::streams::FileIStream>
				file_stream = std::make_unique<common::streams::FileIStream>(file_path);

        if(!file_stream->is_open())
					throw common::diagnostic::InterError(std::format("Can not open file: {}", file_path));

				// creating SourceFile to store in SourcesStorage
				syntax::SourceFile* file = program->sources_storage.add(
						file_path
				,		program->compile_options.memory_page_size
				);

				file->load_from_stream(file_stream.get());

				program->logger.set_name("LEXER");
        // syntax
        syntax::lexer::Lexer lexer(
						file_path
				,		file->content()
				,		&program->logger
				);
        auto tokens = lexer.tokenize();

				program->logger.set_name("PARSER");
        syntax::parser::Parser parser(program->ast_arena.get());
        return parser.parse(tokens);
    }
}
