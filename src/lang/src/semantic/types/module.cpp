#include <lang/semantic/types/identifier.h>
#include <lang/semantic/types/module.h>

namespace lang::semantic
{
    std::unique_ptr<Module> Module::create
    (   std::string_view name
    ,   Scope* global_scope
    ,   std::vector<std::unique_ptr<ast::BaseNode>> ast
    ,   std::vector<std::string> depends
    ){
        auto mdl = std::make_unique<Module>(
            name, 
            std::make_unique<Scope>(global_scope), 
            std::move(ast), 
            std::move(depends)
        );

        return std::move(mdl);
    }

}
