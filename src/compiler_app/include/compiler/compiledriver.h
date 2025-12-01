#pragma once

#include <compiler/cliparser.h>


namespace compiler_app
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
