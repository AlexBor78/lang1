#pragma once

#include <memory>
#include <vector>
#include <unordered_set>
#include <lang/ast/ast.h>
#include <lang/common/symbol_path.h>
#include <lang/semantic/types/scope.h>

// without that clang going crazy with errors, 
// but code still compile IF include that file
#include <lang/semantic/types/identifier.h>

namespace lang::semantic
{
    /**
     * @brief module id, to identify module (bcs name can be repeat)
              and allow to use hash in future easily
        @todo generate hash by default, and use it instead
     */
    struct ModuleID {
        /**
         * @brief absolute path to module
         */
        SymbolPath path;
        // Hash hash; // in future

        // here only for now, i dont want to create ModulePath structure yet
        // operator== check only absolute path (and hash in future)
        SymbolPath relative_path;
        bool is_relative{false};

        // stuff needed by std::map
        bool operator==(const ModuleID& other) const noexcept {
            return  path == other.path;
        }
    };

    struct Module {
        static std::unique_ptr<Module> create(
            SymbolPath path,
            Scope* global_scope,
            std::vector<std::unique_ptr<ast::BaseNode>> ast = {},
            std::vector<ModuleID> depends = {},
            std::unordered_set<ast::BaseNode*> exports = {}
        );

        SymbolPath path;
        ast::AST ast;

        /**
         * @todo optimize (std::vector here is better/equal as std::unordered_set, so only optimization: sort it, and then us binary search)
         */
        // std::vector<std::unique_ptr<ast::BaseNode>
        std::vector<ModuleID> dependencies;
        std::unique_ptr<Scope> scope{nullptr};

        /**
         * @brief temporary list for exports, then it will be in scopes
         * 
         */
        std::unordered_set<ast::BaseNode*> export_list;

        Module(SymbolPath _path
        ,      std::unique_ptr<Scope> _scope
        ,      std::vector<std::unique_ptr<ast::BaseNode>> _ast = {}
        ,      std::vector<ModuleID> depends = {}
        ,      std::unordered_set<ast::BaseNode*> exports = {}
        ):  path(_path)
        ,   ast(std::move(_ast))
        ,   scope(std::move(_scope))
        ,   dependencies(std::move(depends))
        {}
    };
}

namespace std {
    /**
     * @todo: optimize by cache of hash
     */
    template<>
    struct hash<lang::semantic::ModuleID> {
        size_t operator()(const lang::semantic::ModuleID& id) const {
            return hash<lang::SymbolPath>{}(id.path);
        }
    };
}
