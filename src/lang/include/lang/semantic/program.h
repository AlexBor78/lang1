#pragma once

#include <string>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <lang/ast/ast.h>
#include <lang/semantic/scope.h>
#include <lang/semantic/module.h>
#include <lang/semantic/identifier.h>

namespace lang::semantic
{
    struct Program {        
        std::string name{"Main"};
        std::unordered_map<std::string_view, std::unique_ptr<Module>> modules;
        std::unique_ptr<Scope> global_scope{nullptr}; // lang types, etc

        ~Program() = default;
        // copy constructor
        Program(const Program&) = delete;
        Program(Program&& other):
            name(std::move(other.name)),
            modules(std::move(other.modules)),
            global_scope(std::move(other.global_scope))
        {}

        // default constructor
        Program(std::string_view _name = "Main"
        ,       std::unique_ptr<Scope> _global_scope = nullptr
        ,       std::unordered_map<std::string_view, std::unique_ptr<Module>> _modules = {}
        ):  name(_name)
        ,   modules(std::move(_modules))
        ,   global_scope(std::move(_global_scope))
        {}
    };
}
