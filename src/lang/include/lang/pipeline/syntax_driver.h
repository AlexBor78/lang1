#pragma once

#include <lang/semantic/types/semantic_types.h>
#include <vector>

namespace lang::pipeline
{
    class SyntaxDriver
    {
    public:
        explicit SyntaxDriver(semantic::SemanticState* _state
        ,                     const std::vector<std::string> _paths = {}
        ):  state(_state)
        ,   paths(&_paths)
        {}

    private: // var
        semantic::SemanticState* state{nullptr};
        const std::vector<std::string>* paths{nullptr};

    public:
        void set_import_paths(const std::vector<std::string>&);
        
        // receive module name, not file name (will create path of module itself)
        void process(const std::string&);
        void process(const std::string&, semantic::SemanticState*);
    };
}
