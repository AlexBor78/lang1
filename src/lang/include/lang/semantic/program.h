#pragma once

#include <string>
#include <memory>
#include <vector>
#include <string_view>
#include <unordered_map>
#include <lang/ast/ast.h>
#include <lang/semantic/scope.h>
#include <lang/semantic/module.h>
#include <lang/semantic/identifier.h>

namespace lang::semantic
{
    struct Program {
        ~Program();
        static std::unique_ptr<Scope> init_global_scope() noexcept;

        std::string name{"Main"};
        std::unordered_map<std::string_view, std::unique_ptr<Module>> modules;
        std::vector<std::unique_ptr<ast::BaseNode>> global_ast;
        std::unique_ptr<Scope> global_scope{init_global_scope()};

        Program(std::string_view _name = "Main"
        ,       std::unordered_map<std::string_view, std::unique_ptr<Module>> _modules = {}
        ,       std::vector<std::unique_ptr<ast::BaseNode>> _global_ast = {}
        ,       std::unique_ptr<Scope> _global_scope = init_global_scope()
        ):  name(_name)
        ,   modules(std::move(_modules))
        ,   global_ast(std::move(_global_ast))
        ,   global_scope(std::move(_global_scope))
        {}
    };
}
