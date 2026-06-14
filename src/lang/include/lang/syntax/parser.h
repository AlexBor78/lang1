#pragma once

#include <cstddef>
#include <memory>

#include <common/memory/arena_aloc.h>
#include <common/utils/logger.h>

#include <lang/semantic/types/typesystem.h>
#include <lang/utils/diagnostic.h>
#include <lang/syntax/container.h>
#include <lang/syntax/ast/ast.h>
#include <lang/syntax/ast/stmt.h>
#include <lang/syntax/ast/expr.h>
#include <lang/syntax/token.h>

namespace lang::syntax::parser
{
    class Parser {
    public: // api
        Parser(common::memory::ArenaAloc* _arena):
					arena(_arena)
				{}

        SyntaxContainer parse(const std::vector<Token>&);
        bool had_errors() const noexcept;

    private: // vars
        common::utils::Logger logger{"PARSER"};
        SyntaxContainer syntax_container;
				common::memory::ArenaAloc* arena;

        const std::vector<Token>* tokens{nullptr};    
        bool success{true};
        size_t pos{0};
    
    private: // api
        void reset_state();
        void breakpoint();

        void save_type_to_context(DeclName*, AbstractType*);
        void add_to_extern_list(DeclName*);
        void add_to_export_list(DeclStmt*);
        void add_to_imports_list(ImportStmt*);
        void add_to_submodules(ImportStmt*);

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
        StmtPtr process_token();
        StmtNode*          process_stmt();
        
        // moduless
        ImportStmt*        process_import_stmt();

        // control flow
        IfStmt*            process_if_stmt();
        ElseStmt*          process_else_stmt();
        ForStmt*           process_for_stmt();
        WhileStmt*         process_while_stmt();

        // other stmt's
        BlockStmt*         process_scope();
        BreakStmt*         process_break_stmt();
        ContinueStmt*      process_continue_stmt();
        ReturnStmt*        process_return_stmt();

        // declare
        AbstractType*      process_type();
        DeclStmt*          process_declare();
        DeclVariable*      process_variable_decl();
        StackAllocExpr*    process_stackalloc_expr();
        DeclFunction*      process_function_decl();

    // exprs
        ExprNode*          process_expr();
        
        // operators
        ExprNode*          process_operator();
        ExprNode*          process_assign_expr();
        ExprNode*          process_logical_expr();
        ExprNode*          process_compare_expr();
        ExprNode*          process_additive_expr();
        /**
         * @todo rename to process_multiply_expr()
         */
        ExprNode*          process_multiple_expr();
        ExprNode*          process_unary_expr();
        ExprNode*          process_primary_expr();
        
        // names
        ExprNode*          process_name();
        FunctionExpr*      process_function_expr();
        VariableExpr*      process_variable_expr();
        SymbolPathExpr*    process_symbol_path();

        // literals
        LiteralExpr*       process_literal();
        NumberLiteral*     process_number_literal();
        StringLiteral*     process_string_literal();
        BoolLiteral*       process_bool_literal();

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
