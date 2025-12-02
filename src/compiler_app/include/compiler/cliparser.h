#pragma once

#include <string>
#include <vector>
#include <string_view>

#include <lang/pipeline/compile_options.h>

namespace compiler_app
{
    struct ParsedFlag {
        std::string flag;
        std::string value; // optional
    };

    class CLIParser {
    public: // api
        lang::pipeline::CompileOptions process(const std::vector<std::string>&);
    
    private: // var
        lang::pipeline::CompileOptions optoins;
        std::vector<ParsedFlag> flags;
        
    private: // api
        void parse(const std::vector<std::string>&);
        bool flag_requires_value(std::string_view);
        
    };
}
