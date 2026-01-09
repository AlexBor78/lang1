#include <lang/semantic/analyzers/semantic_initializer.h>
#include <lang/semantic/analyzers/symbols_collector.h>
#include <lang/pipeline/semantic_driver.h>

namespace lang::pipeline
{
    void SemanticDriver::analyze() {
        { // initialize semantic state
            semantic::SemanticInitializer initializer(
                compile_options,
                semantic_state
            ); initializer.init();
        }
        return;
        {   // collecting all symbols
            semantic::SymbolsCollector collector(semantic_state);
            collector.collect();
        }
        {   // semantic analyze
            
        }
    }
}
