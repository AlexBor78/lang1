// other drivers
#include <lang/pipeline/modules_loader.h>
#include <lang/pipeline/semantic_driver.h>

// utils
#include <lang/utils/ast_utils.h>
#include <lang/utils/syntax_utils.h>

#include <lang/pipeline/compiledriver.h>

namespace lang::pipeline {
    
    void CompileDriver::run(CompileOptions _options) {
        options = std::move(_options);
        run();
    }

    void CompileDriver::run() {
       auto state = semantic::SemanticInitializer::init_state(options.name);

        {   // parsing all fils (loading them as modules to semantic info)
            ModulesLoader loader(&state);
            loader.load();
            if(options.syntax_only) return;
        }
        
        {   // semantic analyze
            SemanticDriver analyzer(&state);
            analyzer.analyze();
            if(options.semantic_only) return;
        }

        {   // codegen: todo

        }
    }
}
