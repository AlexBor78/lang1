// other drivers
#include <lang/pipeline/modules_loader.h>
#include <lang/pipeline/semantic_driver.h>

// utils
#include <lang/utils/ast_utils.h>
#include <lang/utils/syntax_utils.h>
#include <common/memory/arena_aloc.h>

#include <lang/pipeline/compiledriver.h>

namespace lang::pipeline {
    void CompileDriver::run(CompileOptions _options) {
        compile_options = std::move(_options);
        run();
    }

    void CompileDriver::run()
    {
       program.compile_options = compile_options;

			 // initializing logger
			init_logger(); 

        { // syntax stage
					
					program.logger.set_name("SYNTAX");
					
					// alocating arena for ast
					program.ast_arena = std::make_unique<common::memory::ArenaAloc>(
						program.compile_options.ast_arena_init_size
					);

					// parsing all files (loading them as units to untis mngr)
					ModulesLoader loader(&program);
					loader.load();
					if(compile_options.syntax_only) return;
        }
        return;
        
        {   // semantic analyze
       			semantic::SemanticState semantic_state;
            SemanticDriver analyzer(
                &compile_options,
                &semantic_state
            ); analyzer.analyze();
            if(compile_options.semantic_only) return;
        }

        {   // codegen: todo

        }
    }

		void CompileDriver::init_logger() {
			// todo
			if(program.compile_options.verbose_all
			|| program.compile_options.verbose_syntax
			) program.logger.add_profile("SYNTAX", {
					.level 	= common::utils::Logger::LogLevel::VERBOSE
			});

			if(program.compile_options.verbose_all
			|| program.compile_options.verbose_syntax
			|| program.compile_options.verbose_lexer
			) program.logger.add_profile("LEXER", {
					.level 	= common::utils::Logger::LogLevel::VERBOSE
				,	.prefix = "SYNTAX"
			});

			if(program.compile_options.verbose_all
			|| program.compile_options.verbose_syntax
			|| program.compile_options.verbose_parser
			) program.logger.add_profile("PARSER", {
					.level 	= common::utils::Logger::LogLevel::VERBOSE
				,	.prefix = "SYNTAX"
			});

			if(program.compile_options.trace_lexer)
				program.logger.add_profile("LEXER", {
					.level 	= common::utils::Logger::LogLevel::TRACE
				,	.prefix = "SYNTAX"
			});

			if(program.compile_options.trace_parser)
				program.logger.add_profile("PARSER", {
					.level 	= common::utils::Logger::LogLevel::TRACE
				,	.prefix = "SYNTAX"
			});
			
		}
}
