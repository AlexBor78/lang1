#include <lang/semantic/types/identifier.h>
#include <lang/semantic/types/module.h>
#include <unordered_set>

namespace lang::semantic
{
    std::unique_ptr<Module> Module::create
    (   SymbolPath path
    ,   Scope* global_scope
    ,   std::vector<std::unique_ptr<ast::BaseNode>> ast
    ,   std::vector<ModuleID> depends
    ,   std::unordered_set<ast::BaseNode*> exports
    ){
        auto mdl = std::make_unique<Module>(
            path, 
            std::make_unique<Scope>(global_scope), 
            std::move(ast), 
            std::move(depends),
            std::move(exports)
        ); return std::move(mdl);
    }

}
