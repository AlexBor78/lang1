#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include <lang/ast/ast.h>
#include <lang/ast/expr.h>
#include <lang/ast/stmt.h>
#include <lang/utils/error.h>
#include <lang/utils/logger.h>
#include <lang/frontend/token.h>
#include <lang/semantic/typesystem.h>

namespace lang::frontend::parser
{
    class Parser {
    public: // public api
        Parser() {
            init_logger();
        }

        // return ast
        std::vector<std::unique_ptr<ast::BaseNode>> parse(const std::vector<Token>&);

        // is_success
        bool had_errors() const noexcept;

        // free memory that contains temporary types
        void clear_typecontext() noexcept;

    private: // vars
        utils::Logger logger{utils::Logger::LogLevel::ALL};
        std::vector<std::unique_ptr<Type>> type_context;
        const std::vector<Token>* tokens{nullptr};    
        bool module_declared{false};
        bool success{true};
        size_t pos{0};
    
    private: // private api
        void reset_state();
        void init_logger();
        void breakpoint();

        const Type* add_type(std::unique_ptr<Type>) noexcept;

        bool         is_end(size_t n = 1) const;
        bool         match(TokenType, size_t offset = 0) const;
        const Token& peek(size_t offset = 0) const;
        Token        advance();
        void         putback(size_t n = 1);
        void         skip(size_t n = 1);

    private: // process_ functions
        
    // stmts
        void process_semicolon();
        ast::StmtPtr process_token();
        std::unique_ptr<ast::StmtNode>          process_stmt();
        
        // modules
        std::unique_ptr<ast::DeclModule>        process_module_decl();
        std::unique_ptr<ast::ImportStmt>        process_import_stmt();

        // control flow
        std::unique_ptr<ast::IfStmt>            process_if_stmt();
        std::unique_ptr<ast::ElseStmt>          process_else_stmt();
        std::unique_ptr<ast::ForStmt>           process_for_stmt();
        std::unique_ptr<ast::WhileStmt>         process_while_stmt();

        // other stmt's
        std::unique_ptr<ast::BlockStmt>         process_scope();
        std::unique_ptr<ast::BreakStmt>         process_break_stmt();
        std::unique_ptr<ast::ContinueStmt>      process_continue_stmt();
        std::unique_ptr<ast::ReturnStmt>        process_return_stmt();

        // declare
        QualType process_type();
        std::unique_ptr<ast::DeclStmt>          process_declare();
        std::unique_ptr<ast::DeclNamespace>     process_namespace_decl();
        std::unique_ptr<ast::DeclVariable>      process_variable_decl();
        std::unique_ptr<ast::DeclFunction>      process_function_decl();

    // exprs
        std::unique_ptr<ast::ExprNode>          process_expr();
        
        // operators
        std::unique_ptr<ast::ExprNode>          process_operator();
        std::unique_ptr<ast::ExprNode>          process_assign_expr();
        std::unique_ptr<ast::ExprNode>          process_logical_expr();
        std::unique_ptr<ast::ExprNode>          process_compare_expr();
        std::unique_ptr<ast::ExprNode>          process_additive_expr();
        std::unique_ptr<ast::ExprNode>          process_multiple_expr();
        std::unique_ptr<ast::ExprNode>          process_unary_expr();
        std::unique_ptr<ast::ExprNode>          process_primary_expr();
        
        // names
        std::unique_ptr<ast::ExprNode>          process_name();
        std::unique_ptr<ast::FunctionExpr>      process_function_expr();
        std::unique_ptr<ast::VariableExpr>      process_variable_expr();
        std::unique_ptr<ast::NamespaceExpr>     process_namespace_expr();

        // literals
        std::unique_ptr<ast::LiteralExpr>       process_literal();
        std::unique_ptr<ast::NumberLiteral>     process_number_literal();
        std::unique_ptr<ast::StringLiteral>     process_string_literal();
        std::unique_ptr<ast::BoolLiteral>       process_bool_literal();

    private: // errors creating

        // Inter errors
        errors::ParserError tokens_nullptr(size_t offset = 0) const noexcept;
        errors::ParserError peek_out_of_range(size_t offset = 0) const noexcept;
        errors::ParserError putback_out_of_range(size_t offset = 0) const noexcept;
        errors::ParserError end_reached(size_t offset = 0) const noexcept;

        // other errors
        errors::ParserError multiple_module_decl_in_file(size_t offset = 0) const noexcept;
        errors::ParserError is_end_with_zero(size_t offset = 0) const noexcept;
        errors::ParserError strcut_is_not_suported(size_t offset = 0) const noexcept;
        errors::ParserError enum_is_not_suported(size_t offset = 0) const noexcept;
        errors::ParserError if_is_not_suported(size_t offset = 0) const noexcept;
        errors::ParserError else_is_not_suported(size_t offset = 0) const noexcept;
        errors::ParserError for_is_not_suported(size_t offset = 0) const noexcept;
        errors::ParserError while_is_not_suported(size_t offset = 0) const noexcept;
        errors::ParserError break_is_not_suported(size_t offset = 0) const noexcept;
        errors::ParserError continue_is_not_suported(size_t offset = 0) const noexcept;

        // expected X
        errors::ParserError expected_add_op(size_t offset = 0) const noexcept;
        errors::ParserError expected_mul_op(size_t offset = 0) const noexcept;
        errors::ParserError expected_unary_op(size_t offset = 0) const noexcept;
        errors::ParserError expected_postfix_op(size_t offset = 0) const noexcept;
        errors::ParserError expected_identifier(size_t offset = 0) const noexcept;
        errors::ParserError expected_namespace_name(size_t offset = 0) const noexcept;
        errors::ParserError expected_module_name(size_t offset = 0) const noexcept;
        errors::ParserError expected_variable_name(size_t offset = 0) const noexcept;
        errors::ParserError expected_function_name(size_t offset = 0) const noexcept;
        errors::ParserError expected_doublecolon(size_t offset = 0) const noexcept;
        errors::ParserError expected_semicolon(size_t offset = 0) const noexcept;
        errors::ParserError expected_number(size_t offset = 0) const noexcept;
        errors::ParserError expected_string(size_t offset = 0) const noexcept;
        errors::ParserError expected_bool(size_t offset = 0) const noexcept;
        errors::ParserError expected_comma(size_t offset = 0) const noexcept;
        errors::ParserError expected_type(size_t offset = 0) const noexcept;
        errors::ParserError expected_lbrace(size_t offset = 0) const noexcept;
        errors::ParserError expected_rbrace(size_t offset = 0) const noexcept;
        errors::ParserError expected_lparen(size_t offset = 0) const noexcept;
        errors::ParserError expected_rparen(size_t offset = 0) const noexcept;
        errors::ParserError unexpected_token(size_t offset = 0) const noexcept;
    };
}
