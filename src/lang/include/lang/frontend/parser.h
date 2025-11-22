#pragma once

#include <memory>
#include <vector>

#include <lang/ast/ast.h>
#include <lang/ast/expr.h>
#include <lang/ast/stmt.h>
#include <lang/utils/error.h>
#include <lang/frontend/token.h>
#include <lang/semantic/typesystem.h>

namespace lang::frontend::parser
{
    class Parser {
    private:
        std::vector<std::unique_ptr<ast::BaseNode>> ast;
        const std::vector<Token>* tokens{nullptr};
        CompileError error = {"", Position()};
        
        TypeTable typetable;
        bool module_declared{false};
        size_t pos{0};

        std::vector<std::unique_ptr<Type>> type_context;
    
    private:
        bool is_end() const;
        bool parse();

        const Type* add_type(std::unique_ptr<Type>) noexcept;

        const Token& curr_tok() const;
        const Token& peak_tok(size_t offset = 1) const;
        const Token& next_tok(size_t offset = 1);
        const Token& back_tok(size_t offset = 1);

        CompileError tokens_nullptr(size_t offset = 0) const noexcept;
        CompileError multiple_module_decl_in_file(size_t offset = 0) const noexcept;
        CompileError strcut_is_not_suported(size_t offset = 0) const noexcept;
        CompileError enum_is_not_suported(size_t offset = 0) const noexcept;
        CompileError if_is_not_suported(size_t offset = 0) const noexcept;
        CompileError else_is_not_suported(size_t offset = 0) const noexcept;
        CompileError for_is_not_suported(size_t offset = 0) const noexcept;
        CompileError while_is_not_suported(size_t offset = 0) const noexcept;
        CompileError break_is_not_suported(size_t offset = 0) const noexcept;
        CompileError continue_is_not_suported(size_t offset = 0) const noexcept;

        CompileError expected_add_op(size_t offset = 0) const noexcept;
        CompileError expected_mul_op(size_t offset = 0) const noexcept;
        CompileError expected_unary_op(size_t offset = 0) const noexcept;
        CompileError expected_postfix_op(size_t offset = 0) const noexcept;
        CompileError expected_identifier(size_t offset = 0) const noexcept;
        CompileError expected_namespace_name(size_t offset = 0) const noexcept;
        CompileError expected_module_name(size_t offset = 0) const noexcept;
        CompileError expected_variable_name(size_t offset = 0) const noexcept;
        CompileError expected_function_name(size_t offset = 0) const noexcept;
        CompileError expected_doublecolon(size_t offset = 0) const noexcept;
        CompileError expected_semicolon(size_t offset = 0) const noexcept;
        CompileError expected_number(size_t offset = 0) const noexcept;
        CompileError expected_string(size_t offset = 0) const noexcept;
        CompileError expected_bool(size_t offset = 0) const noexcept;
        CompileError expected_comma(size_t offset = 0) const noexcept;
        CompileError expected_type(size_t offset = 0) const noexcept;
        CompileError expected_lbrace(size_t offset = 0) const noexcept;
        CompileError expected_rbrace(size_t offset = 0) const noexcept;
        CompileError expected_lparen(size_t offset = 0) const noexcept;
        CompileError expected_rparen(size_t offset = 0) const noexcept;
        CompileError unexpected_token(size_t offset = 0) const noexcept;
        
    private: // process_ functions
        
    // stmts
        void process_semicolon();
        ast::StmtPtr process_token();
        std::unique_ptr<ast::StmtNode>          process_stmt();
        
        std::unique_ptr<ast::BlockStmt>         process_scope();
        // std::unique_ptr<ast::IfStmt>         process_if_stmt();
        // std::unique_ptr<ast::ForStmt>        process_for_stmt();
        // std::unique_ptr<ast::WhileStmt>      process_while_stmt();
        // std::unique_ptr<ast::BreakStmt>      process_break_stmt();
        // std::unique_ptr<ast::ContinueStmt>   process_continue_stmt();
        std::unique_ptr<ast::ImportStmt>        process_import_stmt();
        std::unique_ptr<ast::ReturnStmt>        process_return_stmt();

        // declare
        QualType process_type();
        std::unique_ptr<ast::DeclStmt>          process_declare();
        std::unique_ptr<ast::DeclModule>        process_module_decl();
        std::unique_ptr<ast::DeclVariable>      process_variable_decl();
        std::unique_ptr<ast::DeclFunction>      process_function_decl();
        std::unique_ptr<ast::DeclNamespace>     process_namespace_decl();

    // exprs
        std::unique_ptr<ast::ExprNode>          process_expr();
        
        std::unique_ptr<ast::ExprNode>          process_operator();
        std::unique_ptr<ast::ExprNode>          process_assign_expr();
        std::unique_ptr<ast::ExprNode>          process_logical_expr();
        std::unique_ptr<ast::ExprNode>          process_compare_expr();
        std::unique_ptr<ast::ExprNode>          process_additive_expr();
        std::unique_ptr<ast::ExprNode>          process_multiple_expr();
        std::unique_ptr<ast::ExprNode>          process_unary_expr();
        std::unique_ptr<ast::ExprNode>          process_primary_expr();
        
        std::unique_ptr<ast::ExprNode>          process_name();
        std::unique_ptr<ast::FunctionExpr>      process_function_expr();
        std::unique_ptr<ast::VariableExpr>      process_variable_expr();
        std::unique_ptr<ast::NamespaceExpr>     process_namespace_expr();

        std::unique_ptr<ast::LiteralExpr>       process_literal();
        std::unique_ptr<ast::NumberLiteral>     process_number_literal();
        std::unique_ptr<ast::StringLiteral>     process_string_literal();
        std::unique_ptr<ast::BoolLiteral>       process_bool_literal();
        
    public:
        void reset() noexcept;

        // return true if error
        bool load_and_parse(const std::vector<Token>&);

        std::vector<std::unique_ptr<ast::BaseNode>>  get_program() noexcept;
        CompileError get_error() const noexcept;
    };
}
