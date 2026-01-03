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
       auto semantic_state = semantic::SemanticInitializer::init_state(compile_options.output_name);

        {   // parsing all files (loading them as modules to semantic info)
            ModulesLoader loader(
                &compile_options,
                &compile_state,
                &semantic_state
            ); loader.load();
            if(compile_options.syntax_only) return;
        }
        
        {   // semantic analyze
            SemanticDriver analyzer(&semantic_state);
            analyzer.analyze();
            if(compile_options.semantic_only) return;
        }

        {   // codegen: todo

        }
    }
}
