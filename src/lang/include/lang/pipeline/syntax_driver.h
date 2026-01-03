#pragma once

#include <lang/syntax/syntax_container.h>
#include <lang/pipeline/compile_state.h>
#include <lang/pipeline/compile_options.h>
#include <lang/semantic/types/semantic_types.h>
#include <lang/semantic/types/module.h>

namespace lang::pipeline
{
    class SyntaxDriver
    {
    public:
        explicit SyntaxDriver(const CompileOptions* _compile_options // unused for now
        ,                     CompileState* _compile_state
        ,                     semantic::SemanticState* _semantic_state
        ):  compile_options(_compile_options)
        ,   compile_state(_compile_state)
        ,   semantic_state(_semantic_state)
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
        const CompileOptions* compile_options;
        CompileState* compile_state{nullptr};
        semantic::SemanticState* semantic_state{nullptr};
    };
}
