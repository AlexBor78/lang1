#include <format>
#include <stdexcept>
#include <compiler/cliparser.h>

namespace compiler_app {
    lang::CompileOptions CLIParser::process(const std::vector<std::string>& args)
    {
        parse(args);

        // TODO: write --help
        // usage: compiler [options]
        // in options only write paths to find file, it will find main.lang itself
        

        // processing flags
        for(const auto& flag : flags) {
            if(flag.flag == "-o" || flag.flag == "--output") {
                options.output_name = flag.value;
                continue;
            }
            if(flag.flag == "-I" || flag.flag == "--import-path") {
                options.import_paths.emplace_back(flag.value);
                continue;
            }

            // features 
						// TODO: --help
            if(flag.flag == "-h" || flag.flag == "--help") {
                continue;
            }
						if(flag.flag == "--dry-run") {
							options.dry_run = true;
							continue;
						}
            if(flag.flag == "--syntax-only") {
                options.syntax_only = true;
                continue;
            }
            if(flag.flag == "--semantic-only") {
                options.semantic_only = true;
                continue;
            }

            // debug
            if(flag.flag == "--print-tokens") {
                options.print_modules_tokens.emplace_back(flag.value);
                continue;
            }
            if(flag.flag == "--print-ast") {
                options.print_modules_ast.emplace_back(flag.value);
                continue;
            }

						// verbose
						if(flag.flag == "-v" || flag.flag == "--verbose") {
								options.verbose_all = true;							
								continue;
						}
						if(flag.flag == "--verbose-syntax") {
								options.verbose_syntax = true;							
								continue;
						}
						if(flag.flag == "--verbose-lexer") {
								options.verbose_lexer = true;
								continue;
						}
						if(flag.flag == "--verbose-parser") {
								options.verbose_parser = true;
								continue;
						}

						// trace
            if(flag.flag == "--trace-lexer") {
                options.trace_lexer = true;
                continue;
            }
            if(flag.flag == "--trace-parser") {
                options.trace_parser = true;
                continue;
            }


						// other options
						if(flag.flag == "--memory-page-size") {
								options.memory_page_size = std::stoull(flag.value);
								continue;
						}
						if(flag.flag == "--ast--arena-init-size") {
								options.ast_arena_init_size = std::stoull(flag.value);
								continue;
						}

            throw std::runtime_error(std::format("unknown flag: {}, use --help", flag.flag));
        }
        return std::move(options);
    }

    void CLIParser::parse(const std::vector<std::string>& args) {
        for(size_t i = 0; i < args.size(); ++i) {
            std::string_view arg = args[i];
            ParsedFlag flag;
            if(arg[0] == '-') { // if it is flag
                if(arg.size() == 1) throw std::runtime_error("unexpected -, use --help");
                flag.flag = arg;
                if(flag_requires_value(arg)) {
                    if(args.size() < i++) throw std::runtime_error(
												std::format("after flag {} expected value, use --help", arg)
										); flag.value = args[i];
                } flags.emplace_back(std::move(flag));
            } else options.inputs_files.emplace_back(arg);
        }
    }

    bool CLIParser::flag_requires_value(std::string_view flag) {
        return flag == "--output" 		 || flag == "-o"
        ||     flag == "--import-path" || flag == "-I"
				||		 flag == "--print-tokens"
				|| 		 flag == "--print-ast"
				||		 flag == "--memory-page-size"
				||		 flag == "--ast-arena-init-size"
				;
    }
}
