#pragma once

#include <lang/common/compile/program.h>
#include <lang/syntax/syntax_container.h>

namespace lang::pipeline
{
    class SyntaxDriver
    {
    public:
        explicit SyntaxDriver(Program* _program
        ):  program(_program)
        {}

        // receive file name
        /**
         * @brief process file, return ModuleID of file (one file = one module)
         * @param file file path
         * @return semantic::ModuleID 
         */
        syntax::SyntaxContainer process_file(const std::string&);

        /**
         * @brief look for module, then process
         * @param module_id id of module
         * @return semantic::ModuleID 
         */
        semantic::ModuleID process_module(const semantic::ModuleID&);

    private: // var
        Program* program;
    };
}
