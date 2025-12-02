#include <format>
#include <stdexcept>
#include <compiler/cliparser.h>

namespace compiler_app {
    lang::pipeline::CompileOptions CLIParser::process(const std::vector<std::string>& args)
    {
        parse(args); // btw add main files   

        // processing flags
        for(const auto& flag : flags) {
            if(flag.flag == "-o" || flag.flag == "--output") {
                optoins.name = flag.value;
                continue;
            }
            if(flag.flag == "-I" || flag.flag == "--import-path") {
                optoins.import_paths.emplace_back(flag.value);
                continue;
            }

            // features 
            if(flag.flag == "-h" || flag.flag == "--help") {
                optoins.help = true;
                continue;
            }
            if(flag.flag == "--syntax-only") {
                optoins.syntax_only = true;
                continue;
            }
            if(flag.flag == "--semantic-only") {
                optoins.semantic_only = true;
                continue;
            }

            // debug
            if(flag.flag == "--trace-lexer") {
                optoins.trace_lexer = true;
                continue;
            }
            if(flag.flag == "--trace-parser") {
                optoins.trace_parser = true;
                continue;
            }
            if(flag.flag == "--print-tokens") {
                optoins.print_tokens = true;
                continue;
            }
            if(flag.flag == "--print-ast") {
                optoins.print_ast = true;
                continue;
            }

            throw std::runtime_error(std::format("unknown flag: {}, use --help", flag.flag));
        }
        return std::move(optoins);
    }

    void CLIParser::parse(const std::vector<std::string>& args) {
        for(size_t i = 0; i < args.size(); ++i) {
            std::string_view arg = args[i];
            ParsedFlag flag;
            if(arg[0] == '-') { // if it is flag
                if(arg.size() == 1) throw std::runtime_error("unexpected -, use --help");
                flag.flag = arg;
                if(flag_requires_value(arg)) {
                    if(args.size() < i++) throw std::runtime_error(std::format("after flag {} expected value, use --help", arg));
                    flag.value = args[i];
                }
                flags.emplace_back(std::move(flag));
            } else {
                if(!optoins.main_file.empty()) throw std::runtime_error(std::format("unexpected value or file: {}, use --help", arg));
                optoins.main_file = arg;
            }
        }
    }

    bool CLIParser::flag_requires_value(std::string_view flag) {
        return flag == "-o" || flag == "--output"
        ||     flag == "-I" || flag == "--import-path";
    }
}
