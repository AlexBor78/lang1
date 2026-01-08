#pragma once

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include <lang/ast/ast.h>
#include <lang/ast/stmt.h>
#include <lang/ast/expr.h>
#include <lang/utils/diagnostic.h>
#include <common/utils/logger.h>
#include <lang/syntax/token.h>
#include <lang/semantic/types/typesystem.h>
#include <lang/syntax/syntax_container.h>

namespace lang::syntax::parser
{
    class Parser {
    public: // api
        Parser() {
            init();
        }

        SyntaxContainer parse(const std::vector<Token>&);
        bool had_errors() const noexcept;

    private: // vars
        common::utils::Logger logger{common::utils::Logger::LogLevel::ALL};
        std::unordered_map<ast::BaseNode*, std::unique_ptr<AbstractType>> types_context;
        std::unordered_set<ast::BaseNode*> export_list;
        std::unordered_set<ast::BaseNode*> extern_list;

        const std::vector<Token>* tokens{nullptr};    
        // bool module_declared{false}; // deprecated
        bool success{true};
        size_t pos{0};
    
    private: // api
        void init();
        void init_logger();
        void reset_state();
        void breakpoint();

        void save_type_to_context(ast::DeclStmt*, std::unique_ptr<AbstractType>);
        void add_to_extern_list(ast::DeclStmt*);
        void add_to_export_list(ast::DeclStmt*);

        bool         is_end(size_t n = 1) const;
        bool         match(TokenType, size_t offset = 0) const;
        const Token& peek(size_t offset = 0) const;
        Token        advance();
        void         putback(size_t n = 1);
        void         skip(size_t n = 1);

        // cursor movements
        size_t  save_pos();
        void   load_pos(size_t);

    private: // look_like function

        bool look_like_declare();

    private: // process_ functions
        
    // stmts
        void process_semicolon();
        ast::StmtPtr process_token();
        std::unique_ptr<ast::StmtNode>          process_stmt();
        
        // moduless
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
        std::unique_ptr<AbstractType>           process_type();
        std::unique_ptr<ast::DeclStmt>          process_declare();
        std::unique_ptr<ast::DeclVariable>      process_variable_decl();
        std::unique_ptr<ast::StackAllocExpr>    process_stackalloc_expr();
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
        std::unique_ptr<ast::SymbolPathExpr>    process_symbol_path();

        // literals
        std::unique_ptr<ast::LiteralExpr>       process_literal();
        std::unique_ptr<ast::NumberLiteral>     process_number_literal();
        std::unique_ptr<ast::StringLiteral>     process_string_literal();
        std::unique_ptr<ast::BoolLiteral>       process_bool_literal();

    private: // diagnostic creating

        // Inter diagnostic
        diagnostic::ParserError tokens_nullptr(size_t offset = 0) const noexcept;
        diagnostic::ParserError peek_out_of_range(size_t offset = 0) const noexcept;
        diagnostic::ParserError putback_out_of_range(size_t offset = 0) const noexcept;
        diagnostic::ParserError end_reached(size_t offset = 0) const noexcept;

        // other diagnostic
        diagnostic::ParserError is_end_with_zero(size_t offset = 0) const noexcept;
        diagnostic::ParserError strcut_is_not_suported(size_t offset = 0) const noexcept;
        diagnostic::ParserError enum_is_not_suported(size_t offset = 0) const noexcept;
        diagnostic::ParserError if_is_not_suported(size_t offset = 0) const noexcept;
        diagnostic::ParserError else_is_not_suported(size_t offset = 0) const noexcept;
        diagnostic::ParserError for_is_not_suported(size_t offset = 0) const noexcept;
        diagnostic::ParserError while_is_not_suported(size_t offset = 0) const noexcept;
        diagnostic::ParserError break_is_not_suported(size_t offset = 0) const noexcept;
        diagnostic::ParserError continue_is_not_suported(size_t offset = 0) const noexcept;
        diagnostic::ParserError stack_initialization_not_supported(size_t offset = 0) const noexcept;

        // expected X
        diagnostic::ParserError expected_add_op(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_mul_op(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_unary_op(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_postfix_op(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_identifier(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_module_name(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_submodule_name(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_variable_name(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_function_name(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_doublecolon(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_semicolon(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_number(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_string(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_bool(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_comma(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_type(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_lbracket(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_rbracket(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_lbrace(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_rbrace(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_lparen(size_t offset = 0) const noexcept;
        diagnostic::ParserError expected_rparen(size_t offset = 0) const noexcept;
        
        diagnostic::ParserError unexpected_token(size_t offset = 0) const noexcept;
    };
}
