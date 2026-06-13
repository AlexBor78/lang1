#include <filesystem>
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
    //syntax::SyntaxContainer SyntaxDriver::process_file(const std::string& file_path) {
    std::unordered_set<syntax::ImportStmt*> SyntaxDriver::process_file(
				const std::string& file_path) {

        // tring to open file
        std::unique_ptr<common::streams::FileIStream>
				file_stream = std::make_unique<common::streams::FileIStream>(file_path);

        if(!file_stream->is_open())
					throw common::diagnostic::InterError(std::format("Can not open file: {}", file_path));

				const size_t page_size = program->compile_options.memory_page_size;
				const size_t file_size = std::filesystem::file_size(file_path);
				const size_t arena_size = std::max(page_size, ((file_size * 2 + page_size - 1) / page_size) * page_size);

				// creating and loading SourceFile to SourcesStorage
				syntax::SourceFile* file = program->sources_storage.add(
						file_path
				,		file_size
				,		program->compile_options.memory_page_size
				); file->load_from_stream(file_stream.get());

				// tokenizing
				program->logger.set_name("LEXER");
        syntax::lexer::Lexer lexer(
						file_path // need for right SourceLocation generation
				,		file->content()
				,		&program->logger
				); auto tokens = lexer.tokenize();

				// parsing
				// allocating arena for ast
				auto arena = std::make_unique<common::memory::ArenaAloc>(arena_size);
        syntax::parser::Parser parser(
				//	&program->logger
						arena.get()
				); auto result = parser.parse(tokens);

				// storing data in UnitsStorage
				auto unit = program->units_storage.add();
				unit->file_id = file->get_id();
				unit->ast_arena = std::move(arena);
				unit->ast = std::move(result.ast);
				program->logger.set_name("PARSER");

				auto imports = result.imports_list;
				imports.insert(
						result.submodules_list.begin()
				,		result.submodules_list.end()
				); return imports;
    }
}
