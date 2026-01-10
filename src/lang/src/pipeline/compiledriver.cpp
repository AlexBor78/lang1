// other drivers
#include <lang/pipeline/modules_loader.h>
#include <lang/pipeline/semantic_driver.h>

// utils
#include <lang/utils/ast_utils.h>
#include <lang/utils/syntax_utils.h>

#include <lang/pipeline/compiledriver.h>

namespace lang::pipeline {
    void CompileDriver::run(CompileOptions _options) {
        compile_options = std::move(_options);
        run();
    }

    void CompileDriver::run()
    {
       semantic::SemanticState semantic_state;
       Program program;
       program.compile_options = compile_options;

        {   // parsing all files (loading them as modules to semantic info)
            ModulesLoader loader(&program);
            loader.load();
            if(compile_options.syntax_only) return;
        }
        return;
        
        {   // semantic analyze
            SemanticDriver analyzer(
                &compile_options,
                &semantic_state
            ); analyzer.analyze();
            if(compile_options.semantic_only) return;
        }

        {   // codegen: todo

        }
    }
}
