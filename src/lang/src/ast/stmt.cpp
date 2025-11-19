#include <ast/visitor.h>

#include <ast/stmt.h>

namespace lang::ast
{
// BlockStmt

    void BlockStmt::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_block_stmt(*this);
    }
    void BlockStmt::accept(NodeVisitor& visitor) noexcept
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

    void IfStmt::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_if_stmt(*this);
    }
    void IfStmt::accept(NodeVisitor& visitor) noexcept
    {
        visitor.visit_if_stmt(*this);
    }

// ForStmt

    void ForStmt::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_for_stmt(*this);
    }
    void ForStmt::accept(NodeVisitor& visitor) noexcept
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

    void WhileStmt::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_while_stmt(*this);
    }
    void WhileStmt::accept(NodeVisitor& visitor) noexcept
    {
        visitor.visit_while_stmt(*this);
    }

// DeclVar

    void DeclVar::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_decl_var(*this);
    }
    void DeclVar::accept(NodeVisitor& visitor) noexcept
    {
        visitor.visit_decl_var(*this);
    }

// DeclFunc

    void DeclFunc::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_decl_func(*this);
    }
    void DeclFunc::accept(NodeVisitor& visitor) noexcept
    {
        visitor.visit_decl_func(*this);
    }

    const std::vector<StmtPtr>& DeclFunc::get_args() const noexcept
    {
        return args;
    }

    const StmtNode* DeclFunc::get_body() const noexcept
    {
        return body.get();
    }

// DeclModule

    void DeclModule::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_decl_module(*this);
    }
    void DeclModule::accept(NodeVisitor& visitor) noexcept
    {
        visitor.visit_decl_module(*this);
    }

    std::string_view DeclModule::get_name() const noexcept
    {
        return name;
    }

// DeclNamespace

    void DeclNamespace::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_decl_namespace(*this);
    }
    void DeclNamespace::accept(NodeVisitor& visitor) noexcept
    {
        visitor.visit_decl_namespace(*this);
    }

    std::string_view DeclNamespace::get_name() const noexcept
    {
        return name;
    }

// ImportStmt

    void ImportStmt::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_import_stmt(*this);
    }
    void ImportStmt::accept(NodeVisitor& visitor) noexcept
    {
        visitor.visit_import_stmt(*this);
    }

    std::string_view ImportStmt::get_imported() const noexcept
    {
        return imported;
    }

// ReturnStmt

    void ReturnStmt::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_return_stmt(*this);
    }
    void ReturnStmt::accept(NodeVisitor& visitor) noexcept
    {
        visitor.visit_return_stmt(*this);
    }

    const ExprNode* ReturnStmt::get_ret() const noexcept
    {
        return ret.get();
    }
}
