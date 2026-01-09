#pragma once

#include <memory>
#include <vector>
#include <unordered_set>
#include <lang/ast/ast.h>
#include <lang/common/symbol_path.h>
#include <lang/semantic/types/scope.h>

// without that clangd going crazy with errors, 
// but code still compile IF include that file
#include <lang/semantic/types/identifier.h>

namespace lang::semantic
{
    /**
     * @brief module id, to identify module (bcs name can repeat)
              and allow to use hash in future easily
     * @todo generate hash by default, and use it instead
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

        /**
         * @brief id of itself, may be needed in future, idk
         */
        ModuleID id;

        /**
         * @brief ast of that module, will be processed in semantic stage
         */
        ast::AST ast;

        /**
         * @brief list of dependencies (not sure will be submodules included)
         * @todo optimize (std::vector here is better/equal as std::unordered_set, so only optimization: sort it, and then us binary search)
         */
        std::vector<ModuleID> dependencies;

        /**
         * @brief scope with exported identifiers
         */
        std::unique_ptr<Scope> interface_scope{nullptr};
        
        /**
         * @brief scope with internal stuff, not exported, not available out module
         */
        std::unique_ptr<Scope> internal_scope{nullptr};

        /**
         * @brief list for exports, then it will be in scopes
         * @note not sure will be here vector + binary search better
         */
        std::unordered_set<ast::BaseNode*> export_list;

        /**
         * @brief list of submodules
         * @todo sort and use binary search
         */
        std::vector<ModuleID> submodules;

        Module(ModuleID _id
        ,      std::vector<std::unique_ptr<ast::BaseNode>> _ast
        ,      std::vector<ModuleID> depends = {}
        ,      std::vector<ModuleID> _submodules = {}
        ,      std::unordered_set<ast::BaseNode*> exports = {}
        ,      std::unique_ptr<Scope> _inner = nullptr
        ):  id(_id)
        ,   ast(std::move(_ast))
        ,   dependencies(std::move(depends))
        ,   submodules(std::move(_submodules))
        ,   export_list(std::move(exports))
        ,   interface_scope(std::move(_inner))
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
