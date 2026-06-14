#pragma once

#include <format>
#include <string>
#include <string_view>
#include <lang/syntax/ast/visitor.h>

namespace lang::utils
{
    class PrintAST : public syntax::ConstASTVisitor
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
        virtual void visit_block_stmt(const syntax::BlockStmt&) noexcept override;
        virtual void visit_if_stmt(const syntax::IfStmt&) noexcept override;
        virtual void visit_else_stmt(const syntax::ElseStmt&) noexcept override;
        virtual void visit_for_stmt(const syntax::ForStmt&) noexcept override;
        virtual void visit_while_stmt(const syntax::WhileStmt&) noexcept override;
        virtual void visit_decl_var(const syntax::DeclVariable&) noexcept override;
        virtual void visit_decl_func(const syntax::DeclFunction&) noexcept override;
        virtual void visit_import_stmt(const syntax::ImportStmt&) noexcept override;
        virtual void visit_return_stmt(const syntax::ReturnStmt&) noexcept override;
        virtual void visit_break_stmt(const syntax::BreakStmt&) noexcept override;
        virtual void visit_continue_stmt(const syntax::ContinueStmt&) noexcept override;
        virtual void visit_number_literal(const syntax::NumberLiteral&) noexcept override;
        virtual void visit_string_literal(const syntax::StringLiteral&) noexcept override;
        virtual void visit_bool_literal(const syntax::BoolLiteral&) noexcept override;
        virtual void visit_variable_expr(const syntax::VariableExpr&) noexcept override;
        virtual void visit_stackalloc_expr(const syntax::StackAllocExpr&) noexcept override;
        virtual void visit_symbol_path(const syntax::SymbolPathExpr&) noexcept override;
        virtual void visit_call_expr(const syntax::FunctionExpr&) noexcept override;
        virtual void visit_binop_expr(const syntax::BinOpExpr&) noexcept override;
        virtual void visit_prefix_unary_op_expr(const syntax::PrefixUnaryOpExpr&) noexcept override;
        virtual void visit_postfix_unary_op_expr(const syntax::PostfixUnaryOpExpr&) noexcept override;

        void print(const syntax::BaseNode*) noexcept;
    };
}
