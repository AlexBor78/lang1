#pragma once

#include <memory>
#include <unordered_map>

#include <lang/semantic/scope.h>
#include <lang/semantic/program.h>
#include <lang/semantic/typesystem.h>

namespace lang::semantic
{
    class SemanticContext {
    private: // api
        std::unordered_map<ast::BaseNode*, std::unique_ptr<Scope>> scope_context;
        // std::unordered_map<ast::BaseNode*, std::unique_ptr<QualType>> types_context; // in next commit

    public:
        Scope* get_scope(ast::BaseNode*);
        Scope* add_scope(ast::BaseNode*, Scope* parent_scope);
    };

    class SemanticState {
    public:
        Program program;
        SemanticContext context; // for anonymous BlockStmt
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

    class SemanticInitializer
    {
    public:
        static SemanticState init_state(std::string_view name = "main");

    private:
        static std::unique_ptr<Scope> init_global_scope();
    };
}
