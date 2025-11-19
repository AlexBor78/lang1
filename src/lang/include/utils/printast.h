#pragma once

#include <format>
#include <string>
#include <string_view>
#include <ast/visitor.h>

namespace lang::utils
{
    class PrintAST : public ast::ConstASTVisitor
    {
    private:
        std::string prefix{""};
        bool need_prefix{true};

        void remove_from_prefix(int count = 2) noexcept;
        void add_to_prefix(std::string_view str = "  ") noexcept;
        void clean_prefix() noexcept;


        template<typename... Args>
        void print(std::format_string<Args...> fmt = "", Args&&...) noexcept;

        template<typename... Args>
        void println(std::format_string<Args...> fmt = "", Args&&...) noexcept;

    public:
        virtual void visit_block_stmt(const ast::BlockStmt&) noexcept override;
        virtual void visit_if_stmt(const ast::IfStmt&) noexcept override;
        virtual void visit_for_stmt(const ast::ForStmt&) noexcept override;
        virtual void visit_while_stmt(const ast::WhileStmt&) noexcept override;
        virtual void visit_decl_var(const ast::DeclVar&) noexcept override;
        virtual void visit_decl_func(const ast::DeclFunc&) noexcept override;
        virtual void visit_decl_module(const ast::DeclModule&) noexcept override;
        virtual void visit_decl_namespace(const ast::DeclNamespace&) noexcept override;
        virtual void visit_import_stmt(const ast::ImportStmt&) noexcept override;
        virtual void visit_return_stmt(const ast::ReturnStmt&) noexcept override;
        virtual void visit_number_literal(const ast::NumberLiteral&) noexcept override;
        virtual void visit_string_literal(const ast::StringLiteral&) noexcept override;
        virtual void visit_bool_literal(const ast::BoolLiteral&) noexcept override;
        virtual void visit_variable_expr(const ast::VariableExpr&) noexcept override;
        virtual void visit_namespace_expr(const ast::NamespaceExpr&) noexcept override;
        virtual void visit_call_expr(const ast::CallExpr&) noexcept override;
        virtual void visit_binop_expr(const ast::BinOpExpr&) noexcept override;
        virtual void visit_prefix_unary_op_expr(const ast::PrefixUnaryOpExpr&) noexcept override;
        virtual void visit_postfix_unary_op_expr(const ast::PostfixUnaryOpExpr&) noexcept override;

        void print(const ast::BaseNode*) noexcept;
    };
}
