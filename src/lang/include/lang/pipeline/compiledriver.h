#pragma once

#include <lang/pipeline/compile_options.h>

namespace lang::pipeline
{
    class CompileDriver
    {
    public: //api
        CompileDriver() = default;
        CompileDriver(CompileOptions _options):
            options(std::move(_options))
        {}

        void run();
        void run(CompileOptions);

    private: // var
        CompileOptions options;
        
    private: // api
    };
}
