#include "ast/ast.h"
#include <ast/visitor.h>

#include <ast/stmt.h>

namespace lang::ast
{
// BlockStmt

    void BlockStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_block_stmt(*this);
    }
    void BlockStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_block_stmt(*this);
    }

    const std::vector<StmtPtr>& BlockStmt::get_body() const noexcept
    {
        return m_body;
    }
    std::vector<StmtPtr>& BlockStmt::get_body() noexcept
    {
        return m_body;
    }
    void BlockStmt::add_tobody(StmtPtr ptr) noexcept
    {
        m_body.emplace_back(std::move(ptr));
    }

// StructureStmt

    const ExprNode* StructureStmt::get_cond() const
    {
        return cond.get();
    }
    const StmtNode* StructureStmt::get_body() const
    {
        return body.get();
    }

// IfStmt

    void IfStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_if_stmt(*this);
    }
    void IfStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_if_stmt(*this);
    }

// ForStmt

    void ForStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_for_stmt(*this);
    }
    void ForStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_for_stmt(*this);
    }

    const StmtNode* ForStmt::get_decl() const
    {
        return decl.get();
    }
    const StmtNode* ForStmt::get_incr() const
    {
        return incr.get();
    }

// WhileStmt

    void WhileStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_while_stmt(*this);
    }
    void WhileStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_while_stmt(*this);
    }

// DeclStmt

    std::string_view DeclStmt::get_name() const noexcept
    {
        return name;
    }

// DeclVar

    void DeclVar::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_decl_var(*this);
    }
    void DeclVar::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_decl_var(*this);
    }

    const ExprNode* DeclVar::get_init_expr() const noexcept
    {
        return init_expr.get();
    }

// DeclFunc

    void DeclFunc::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_decl_func(*this);
    }
    void DeclFunc::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_decl_func(*this);
    }

    const std::vector<std::unique_ptr<DeclVar>>& DeclFunc::get_args() const noexcept
    {
        return args;
    }

    const StmtNode* DeclFunc::get_body() const noexcept
    {
        return body.get();
    }

// DeclModule

    void DeclModule::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_decl_module(*this);
    }
    void DeclModule::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_decl_module(*this);
    }

// DeclNamespace

    void DeclNamespace::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_decl_namespace(*this);
    }
    void DeclNamespace::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_decl_namespace(*this);
    }

    const StmtNode* DeclNamespace::get_body() const noexcept
    {
        return body.get();
    }
    
// ImportStmt

    void ImportStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_import_stmt(*this);
    }
    void ImportStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_import_stmt(*this);
    }

    std::string_view ImportStmt::get_imported() const noexcept
    {
        return imported;
    }

// ReturnStmt

    void ReturnStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_return_stmt(*this);
    }
    void ReturnStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_return_stmt(*this);
    }

    const ExprNode* ReturnStmt::get_ret_expr() const noexcept
    {
        return ret_expr.get();
    }
}
