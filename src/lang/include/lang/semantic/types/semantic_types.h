#pragma once

#include <memory>
#include <unordered_map>

#include <lang/semantic/types/scope.h>
#include <lang/common/compile/program.h>
#include <lang/semantic/types/typesystem.h>

namespace lang::semantic
{
    struct SemanticContext {
        /**
         * @brief save almost ALL scopes. 
         * @note algorithm to find scope by SymbolPath:
            1. go to global scope
            2. find Identifier
            3. in Identifier usably there is pointer to Node
                1. if there is: go to context and get scope
                2. if not - it is module, find module by ModuleID in program, in module there is it's scope
         */
        std::unordered_map<ast::BaseNode*, std::unique_ptr<Scope>> scope_context;

        /**
         * @brief save's types of declarations, bcs ast dont
         * @note full types, with wrappers, but BASE type is stored in types_table
         */
        std::unordered_map<ast::BaseNode*, std::unique_ptr<AbstractType>> types_context;
        
        /**
         * @brief table of types, save BASE types only, not conflict with types_context.
         * 
         */
        TypeTable types_table;

        /**
         * @brief one list of all external stuff is a lot more convenient then every in every module, will be used in codegen
         * 
         */
        std::unordered_set<ast::BaseNode*> extern_list;

    public: // some methods 
        // Scope* get_scope(const ast::BaseNode*);
        Scope* add_scope(const ast::BaseNode*, Scope* parent_scope);
    };

    class SemanticState {
    public:
        Program program;
        SemanticContext context;
        Scope* curr_scope{nullptr};

    public: // api
        SemanticState() = default;
        SemanticState(const SemanticState&) = delete;
        SemanticState(SemanticState&& other):
            program(std::move(other.program)),
            context(std::move(other.context)),
            curr_scope(other.curr_scope)
        {}

        void reset();

        // scope
        void enter_scope(Scope*);
        void leave_scope();
    };
}
