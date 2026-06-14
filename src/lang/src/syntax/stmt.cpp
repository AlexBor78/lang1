#include <lang/syntax/ast/visitor.h>
#include <lang/syntax/ast/stmt.h>
#include <iostream>
#include <print>


namespace lang::syntax
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
		
// IfStmt

    void IfStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_if_stmt(*this);
    }
    void IfStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_if_stmt(*this);
    }

// ElseStmt

    void ElseStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_else_stmt(*this);
    }
    void ElseStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_else_stmt(*this);
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

// WhileStmt

    void WhileStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_while_stmt(*this);
    }
    void WhileStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_while_stmt(*this);
    }

// DeclVar

    void DeclVariable::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_decl_var(*this);
    }
    void DeclVariable::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_decl_var(*this);
    }

// DeclFunction

    void DeclFunction::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_decl_func(*this);
    }
    void DeclFunction::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_decl_func(*this);
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

// ReturnStmt

    void ReturnStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_return_stmt(*this);
    }
    void ReturnStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_return_stmt(*this);
    }

// BreakStmt

    void BreakStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_break_stmt(*this);
    }
    void BreakStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_break_stmt(*this);
    }

// ContinueStmt

    void ContinueStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_continue_stmt(*this);
    }
    void ContinueStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_continue_stmt(*this);
    }
    
}
