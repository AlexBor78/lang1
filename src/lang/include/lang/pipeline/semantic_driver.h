#pragma once

#include <lang/semantic/types/semantic_types.h>
#include <lang/pipeline/compile_options.h>

namespace lang::pipeline
{
    class SemanticDriver
    {
    public:
        SemanticDriver(const pipeline::CompileOptions* _compile_options
        ,              semantic::SemanticState* _semantic_state
        ): compile_options(_compile_options)
        ,  semantic_state(_semantic_state)
        {}

        void analyze();

    private: // vars
        const pipeline::CompileOptions* compile_options;
        semantic::SemanticState* semantic_state{nullptr};
    };
}
