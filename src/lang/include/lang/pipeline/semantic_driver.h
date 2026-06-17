#pragma once

#include <lang/common/program.h>

namespace lang::pipeline
{
    class SemanticDriver
    {
    public:
        SemanticDriver(
					Program* _program
        ): program(_program)
        {}

        void analyze();

    private: // vars
				Program* program;
    };
}
