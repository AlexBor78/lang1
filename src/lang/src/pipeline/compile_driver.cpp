// other drivers
#include <lang/pipeline/modules_loader.h>
#include <lang/pipeline/semantic_driver.h>

// utils
#include <common/memory/arena_alloc.h>
#include <common/utils/strings_storage.h>

#include <lang/pipeline/compile_driver.h>

namespace lang::pipeline {
  void CompileDriver::run(CompileOptions _options) {
  	compile_options = std::move(_options);
  	run();
  }

  void CompileDriver::run() {
		init();

    { // syntax stage
			program->logger.set_name("SYNTAX");
			
			// parsing all files
			ModulesLoader loader(program);
			loader.load();

			if(program->compile_options.syntax_only) return;
    }

		// temporary
    return;
    
    {   // semantic analyze
			program->logger.set_name("SEMANTIC");

    	SemanticDriver analyzer(program);
			analyzer.analyze();
    	if(program->compile_options.semantic_only) return;
    }

    {   // codegen: todo

    }
  }

	void CompileDriver::init() {
		auto root_alloc = std::make_unique<common::memory::AsyncPoolAlloc>();
		program = root_alloc->make<Program>(
			std::move(compile_options)
		,	std::move(root_alloc)
		);

		// initializing logger
		init_logger(); 
	}

	void CompileDriver::init_logger() {
		// syntax verbose
		if(program->compile_options.verbose_all
		|| program->compile_options.verbose_syntax
		) program->logger.add_profile("SYNTAX", {
				.level 	= common::utils::Logger::LogLevel::VERBOSE
		});

		// lexer verbose
		if(program->compile_options.verbose_all
		|| program->compile_options.verbose_syntax
		|| program->compile_options.verbose_lexer
		) program->logger.add_profile("LEXER", {
				.level 	= common::utils::Logger::LogLevel::VERBOSE
			,	.prefix = "SYNTAX"
		});

		// parser verbose
		if(program->compile_options.verbose_all
		|| program->compile_options.verbose_syntax
		|| program->compile_options.verbose_parser
		) program->logger.add_profile("PARSER", {
				.level 	= common::utils::Logger::LogLevel::VERBOSE
			,	.prefix = "SYNTAX"
		});

		// lexer trace
		if(program->compile_options.trace_lexer)
			program->logger.add_profile("LEXER", {
				.level 	= common::utils::Logger::LogLevel::TRACE
			,	.prefix = "SYNTAX"
		});

		// parser trace
		if(program->compile_options.trace_parser)
			program->logger.add_profile("PARSER", {
				.level 	= common::utils::Logger::LogLevel::TRACE
			,	.prefix = "SYNTAX"
		});
		
	}
}
