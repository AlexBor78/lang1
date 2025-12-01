#pragma once

#include <memory>
#include <vector>
#include <lang/ast/ast.h>
#include <lang/semantic/types/scope.h>

namespace lang::semantic
{
    struct Module {
        static std::unique_ptr<Module> create(
            std::string_view name,
            Scope* global_scope,
            std::vector<std::unique_ptr<ast::BaseNode>> ast = {},
            std::vector<std::string> depends = {}
        );

        std::string name;
        bool is_init{false};
        std::vector<std::unique_ptr<ast::BaseNode>> ast;
        std::vector<std::string> dependencies; // vector of modules names;
        std::unique_ptr<Scope> scope{nullptr};

        Module(std::string_view _name
        ,      std::unique_ptr<Scope> _scope
        ,      std::vector<std::unique_ptr<ast::BaseNode>> _ast = {}
        ,      std::vector<std::string> depends = {}
        ):  is_init(true)
        ,   name(_name)
        ,   ast(std::move(_ast))
        ,   scope(std::move(_scope))
        ,   dependencies(std::move(depends))
        {}
    };
}