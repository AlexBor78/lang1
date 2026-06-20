#pragma once

#include <common/memory/pool_alloc.h>
#include <lang/common/program.h>

namespace lang::pipeline
{
    class CompileDriver
    {
    public: //api
				// todo: remove default constructor
        CompileDriver() = default;
        CompileDriver(CompileOptions _options):
            compile_options(std::move(_options))
        {}

        void run();
				// todo: remove run(options) func
        void run(CompileOptions);

    private: // var
        CompileOptions compile_options;
				Program* program;
        
    private: // api
				void init();
				void init_logger();
    };
}
