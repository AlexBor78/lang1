#pragma once

#include <string>
#include <vector>
#include <string_view>

#include <lang/common/compile/options.h>

namespace compiler_app
{
    /**
     * @note value is optinal
     */
    struct ParsedFlag {
        std::string flag;
        std::string value; // optional
    };

    class CLIParser {
    public: // api
        lang::CompileOptions process(const std::vector<std::string>&);
    
    private: // var
        lang::CompileOptions optoins;
        std::vector<ParsedFlag> flags;
        
    private: // api
        void parse(const std::vector<std::string>&);
        bool flag_requires_value(std::string_view);
        
    };
}
