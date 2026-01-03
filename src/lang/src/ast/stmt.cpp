#include <lang/ast/visitor.h>
#include <lang/ast/stmt.h>
#include <iostream>
#include <print>


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

    const StmtNode* StructureStmt::get_body() const {
        return body.get();
    }

// StructureStmtWithCond
    
    const ExprNode* StructureStmtWithCond::get_cond() const {
        return cond.get();
    }

    ExprNode* StructureStmtWithCond::get_cond() {
        return cond.get();
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

    const StmtNode* ForStmt::get_decl() const
    {
        return decl.get();
    }
    StmtNode* ForStmt::get_decl()
    {
        return decl.get();
    }
    const StmtNode* ForStmt::get_incr() const
    {
        return incr.get();
    }
    StmtNode* ForStmt::get_incr()
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

    void DeclVariable::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_decl_var(*this);
    }
    void DeclVariable::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_decl_var(*this);
    }

    const ExprNode* DeclVariable::get_init_expr() const noexcept
    {
        return init_expr.get();
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

    const std::vector<std::unique_ptr<DeclVariable>>& DeclFunction::get_args() const noexcept
    {
        return args;
    }

    const StmtNode* DeclFunction::get_body() const noexcept
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
        std::println(std::cerr, "using deprecated ImportStmt::get_imported() method");
        return imported.path[0];
    }

    const ModulePath& ImportStmt::get_path() const noexcept {
        return imported;
    }
    bool ImportStmt::is_relative() const noexcept {
        return imported.is_relative;
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
