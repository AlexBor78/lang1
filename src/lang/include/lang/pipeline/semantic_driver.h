#pragma once

#include <lang/semantic/types/semantic_types.h>

namespace lang::pipeline
{
    class SemanticDriver
    {
    public:
        SemanticDriver(semantic::SemanticState* _state):
            state(_state)
        {}

        void analyze();
        void analyze(semantic::SemanticState*);

    private: // vars
        semantic::SemanticState* state{nullptr};
    };
}
