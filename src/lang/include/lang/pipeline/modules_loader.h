#pragma once

#include <string>
#include <vector>


#include <lang/semantic/types/semantic_types.h>
#include <lang/pipeline/syntax_driver.h>
#include <lang/semantic/analyzers/import_analyzer.h>

namespace lang::pipeline {
    class ModulesLoader
    {
    public: // api
        explicit ModulesLoader(semantic::SemanticState* _state
        ,                      const std::vector<std::string>& _paths = {}
        ):  state(_state)
        ,   paths(&_paths)
        ,   driver(state)
        {}

        void load();
        void load(semantic::SemanticState*);

    private: // vars
        semantic::SemanticState* state{nullptr};
        const std::vector<std::string>* paths{nullptr};
        semantic::ImportAnalyzer import_processor;
        SyntaxDriver driver;

    private: // api
        void load(const std::string&);
        void load(const std::vector<std::string>&);
    };
}
