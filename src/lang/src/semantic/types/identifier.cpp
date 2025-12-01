#include <lang/semantic/types/identifier.h>

namespace lang::semantic
{
    std::unique_ptr<Identifier> Identifier::create(
        std::string_view name,
        Kind kind,
        ast::DeclStmt* decl,
        Scope* parent_scope
    ){
        bool need_inner{false};
        if(kind == Kind::FUNCTOIN || kind == Kind::NAMESPACE)
            need_inner = true;

        if(need_inner) return std::make_unique<Identifier>(name, kind, decl, std::make_unique<Scope>(parent_scope));
        else return std::make_unique<Identifier>(name, kind, decl);
    }
}
