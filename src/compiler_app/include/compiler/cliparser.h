#pragma once

#include <string>
#include <vector>
#include <string_view>

namespace compiler_app {

    struct CompileOptions {
        std::string name{"a.out"};
        std::string main_file{"main.lang"};
        std::vector<std::string> import_paths;
        // int threads{1}; // todo: multi-threading

        // features
        bool help{false};
        bool syntax_only{false};
        bool semantic_only{false};

        // debug flags
        bool trace_lexer{false};
        bool trace_parser{false};
        bool print_tokens{false};
        bool print_ast{false};
    };

    struct ParsedFlag {
        std::string flag;
        std::string value; // optional
    };

    class CLIParser {
    public: // api
        CompileOptions process(const std::vector<std::string>&);
    
    private: // var
        CompileOptions optoins;
        std::vector<ParsedFlag> flags;
        
    private: // api
        void parse(const std::vector<std::string>&);
        bool flag_requires_value(std::string_view);
        
    };
}
