#pragma once

#include <lang/pipeline/compile_state.h>
#include <lang/pipeline/compile_options.h>

namespace lang::pipeline
{
    class CompileDriver
    {
    public: //api
        CompileDriver() = default;
        CompileDriver(CompileOptions _options):
            compile_options(std::move(_options))
        {}

        void run();
        void run(CompileOptions);

    private: // var
        CompileState   compile_state;
        CompileOptions compile_options;
        
    private: // api
    };
}
