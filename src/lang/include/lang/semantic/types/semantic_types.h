#pragma once


#include <unordered_set>

#include <lang/ast/ast.h>
#include <lang/semantic/types/scope.h>
#include <lang/semantic/types/typesystem.h>

namespace lang::semantic
{
    struct SemanticContext {
        /**
         * @brief one list of all external stuff is a lot more convenient then every in every module, will be used in codegen
         * 
         */
        std::unordered_set<ast::BaseNode*> extern_list;
    };

    class SemanticState {
    public:
        SemanticContext context;
        // Scope* curr_scope{nullptr};

    public: // api
        SemanticState() = default;
        SemanticState(const SemanticState&) = delete;
        SemanticState(SemanticState&& other):
            context(std::move(other.context))
            // curr_scope(other.curr_scope)
        {}

        void reset();

        // scope
        // void enter_scope(Scope*);
        // void leave_scope();
    };
}
