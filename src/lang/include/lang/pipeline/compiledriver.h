#pragma once

#include <lang/common/compile/state.h>
#include <lang/common/compile/program.h>

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
        CompileOptions compile_options;
				Program program;
        
    private: // api
				void init_logger();
    };
}
