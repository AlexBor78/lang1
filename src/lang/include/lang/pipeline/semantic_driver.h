#pragma once

#include <lang/semantic/types/semantic_types.h>
#include <lang/common/compile/options.h>

namespace lang::pipeline
{
    class SemanticDriver
    {
    public:
        SemanticDriver(const CompileOptions* _compile_options
        ,              semantic::SemanticState* _semantic_state
        ): compile_options(_compile_options)
        ,  semantic_state(_semantic_state)
        {}

        void analyze();

    private: // vars
        const CompileOptions* compile_options;
        semantic::SemanticState* semantic_state{nullptr};
    };
}
