#include <lang/semantic/analyzers/symbols_collector.h>
#include <lang/pipeline/semantic_driver.h>

namespace lang::pipeline
{
    void SemanticDriver::analyze(semantic::SemanticState* _state) {
        state = _state;
    }

    void SemanticDriver::analyze() {
        {   // collecting all symbols
            semantic::SymbolsCollector collector(state);
            collector.collect();
        }
        {   // semantic analyze
            
        }

    }
}
