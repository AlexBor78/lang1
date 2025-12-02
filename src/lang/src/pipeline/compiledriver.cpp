// syntax
#include <common/streams/istream.h>
#include <lang/syntax/lexer.h>
#include <lang/syntax/parser.h>

// semantic
#include <lang/semantic/types/semantic_types.h>

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
        // syntax (parsing every file)

        

        // semantic
        

        // ImportAnalyzer
        // ..
    
    }
}
