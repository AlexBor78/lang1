#include <memory>
#include <format>
#include <string>
#include <filesystem>

#include <common/diagnostic/diagnostic.h>
#include <common/streams/istream.h>

#include <lang/syntax/source_file.h>
#include <lang/syntax/lexer.h>
#include <lang/syntax/parser.h>

#include <lang/pipeline/syntax_driver.h>

namespace lang::pipeline {
  std::unordered_set<syntax::ast::ImportStmt*> SyntaxDriver::process_file(
		const std::string& file_path) {

    // tring to open file
    std::unique_ptr<common::streams::FileIStream>
		file_stream = std::make_unique<common::streams::FileIStream>(file_path);

    if(!file_stream->is_open())
			throw common::diagnostic::InterError(std::format("Can not open file: {}", file_path));

		const size_t page_size = program->compile_options.memory_page_size;
		const size_t data_size = 
			std::filesystem::file_size(file_path)
		+	file_path.size()
		+	sizeof(syntax::SourceFile)
		;
		const size_t arena_size = std::max(
			((data_size * 2 + page_size - 1) / page_size) * page_size
		,	page_size
		);

		// creating target
		auto target = program->targets_storage.add(
			std::make_unique<common::memory::PoolAlloc>()
		);

		// allocating arena for source file
		// creating and loading SourceFile to SourcesStorage
		target->create_source_arena(arena_size);
		auto file = target->create_source(file_path);
		file->load_from_stream(file_stream.get());

		// tokenizing
		program->logger.set_name("LEXER");
    syntax::lexer::Lexer lexer(
				file_path // need for right SourceLocation generation
		,		file->content()
		,		&program->logger
		); auto tokens = lexer.tokenize();

		// parsing
		// allocating arena for ast
		target->create_bundle_arena(arena_size);
		program->logger.set_name("PARSER");
    syntax::parser::Parser parser(
		//	&program->logger
			target->bundle_arena	
		); auto result = parser.parse(tokens);
		target->create_bundle(std::move(result.ast));

		auto imports = result.imports_list;
		imports.insert(
				result.submodules_list.begin()
		,		result.submodules_list.end()
		); return imports;
  }
}
