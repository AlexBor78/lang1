#pragma once

#include <format>
#include <string>
#include <string_view>
#include <lang/syntax/ast/visitor.h>

namespace lang::utils
{
    class PrintAST : public syntax::ast::ConstASTVisitor
    {
    private:
        std::string prefix{""};
        bool need_prefix{true};

        void remove_from_prefix(size_t count = 2) noexcept;
        void add_to_prefix(std::string_view str = "  ") noexcept;
        void clean_prefix() noexcept;


        template<typename... Args>
        void print(std::format_string<Args...> fmt = "", Args&&...) noexcept;

        template<typename... Args>
        void println(std::format_string<Args...> fmt = "", Args&&...) noexcept;

    public:
        virtual void visit_block_stmt(const syntax::ast::BlockStmt&) noexcept override;
        virtual void visit_if_stmt(const syntax::ast::IfStmt&) noexcept override;
        virtual void visit_else_stmt(const syntax::ast::ElseStmt&) noexcept override;
        virtual void visit_for_stmt(const syntax::ast::ForStmt&) noexcept override;
        virtual void visit_while_stmt(const syntax::ast::WhileStmt&) noexcept override;
        virtual void visit_decl_var(const syntax::ast::DeclVariable&) noexcept override;
        virtual void visit_decl_func(const syntax::ast::DeclFunction&) noexcept override;
        virtual void visit_import_stmt(const syntax::ast::ImportStmt&) noexcept override;
        virtual void visit_return_stmt(const syntax::ast::ReturnStmt&) noexcept override;
        virtual void visit_break_stmt(const syntax::ast::BreakStmt&) noexcept override;
        virtual void visit_continue_stmt(const syntax::ast::ContinueStmt&) noexcept override;
        virtual void visit_number_literal(const syntax::ast::NumberLiteral&) noexcept override;
        virtual void visit_string_literal(const syntax::ast::StringLiteral&) noexcept override;
        virtual void visit_bool_literal(const syntax::ast::BoolLiteral&) noexcept override;
        virtual void visit_variable_expr(const syntax::ast::VariableExpr&) noexcept override;
        virtual void visit_stackalloc_expr(const syntax::ast::StackAllocExpr&) noexcept override;
        virtual void visit_symbol_path(const syntax::ast::SymbolPathExpr&) noexcept override;
        virtual void visit_call_expr(const syntax::ast::FunctionExpr&) noexcept override;
        virtual void visit_binop_expr(const syntax::ast::BinOpExpr&) noexcept override;
        virtual void visit_prefix_unary_op_expr(const syntax::ast::PrefixUnaryOpExpr&) noexcept override;
        virtual void visit_postfix_unary_op_expr(const syntax::ast::PostfixUnaryOpExpr&) noexcept override;

        void print(const syntax::ast::BaseNode*) noexcept;
    };
}
