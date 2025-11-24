#include <print>
#include <format>
#include <string>

#include <lang/utils/frontend_utils.h>
#include <lang/utils/ast_utils.h>
#include <lang/frontend/parser.h>

namespace lang::frontend::parser
{
// api functions

    void Parser::reset() noexcept {
        tokens = nullptr;
        ast.clear();
        pos = 0;
    }

    bool Parser::load_and_parse(const std::vector<Token>& _tokens) {
        module_declared = false;
        tokens = &_tokens;
        pos = 0;
        return parse();
    }

    std::vector<std::unique_ptr<ast::BaseNode>> Parser::get_program() noexcept {
        auto buf = std::move(ast);
        reset();
        return std::move(buf);
    }

    const errors::ParserError Parser::get_error() const noexcept {
        return error;
    }
    errors::ParserError Parser::get_error() noexcept {
        return error;
    }

// create error
    
    errors::ParserError Parser::expected_semicolon(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected ';', got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_lbrace(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected '{{', got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_rbrace(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected '}}', got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_lparen(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected '(', got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_rparen(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected ')', got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_comma(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected ',', got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_doublecolon(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected '::', got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_unary_op(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected unary operator, got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_postfix_op(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected postfix operator, got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_identifier(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected identifier, got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_function_name(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected function name, got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_variable_name(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected variable name, got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_namespace_name(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected namespace name, got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_module_name(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected module name, got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_type(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected type, got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_number(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected number, got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_string(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected string, got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::expected_bool(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected bool, got {}", utils::stringify(peak_tok(offset).ty)), peak_tok(offset).pos);
    }
    errors::ParserError Parser::tokens_nullptr(size_t offset) const noexcept {
        return errors::ParserError("tokens = nullptr", SourceLocation());
    }
    errors::ParserError Parser::strcut_is_not_suported(size_t offset) const noexcept {
        return errors::ParserError("strcut is currently not suported", SourceLocation());
    }
    errors::ParserError Parser::enum_is_not_suported(size_t offset) const noexcept {
        return errors::ParserError("enum is currently not suported", SourceLocation());
    }
    errors::ParserError Parser::if_is_not_suported(size_t offset) const noexcept {
        return errors::ParserError("if is currently not suported", SourceLocation());
    }
    errors::ParserError Parser::else_is_not_suported(size_t offset) const noexcept {
        return errors::ParserError("else is currently not suported", SourceLocation());
    }
    errors::ParserError Parser::for_is_not_suported(size_t offset) const noexcept {
        return errors::ParserError("for is currently not suported", SourceLocation());
    }
    errors::ParserError Parser::while_is_not_suported(size_t offset) const noexcept {
        return errors::ParserError("while is currently not suported", SourceLocation());
    }
    errors::ParserError Parser::break_is_not_suported(size_t offset) const noexcept {
        return errors::ParserError("break is currently not suported", SourceLocation());
    }
    errors::ParserError Parser::continue_is_not_suported(size_t offset) const noexcept {
        return errors::ParserError("continue is currently not suported", SourceLocation());
    }
    errors::ParserError Parser::multiple_module_decl_in_file(size_t offset) const noexcept {
        return errors::ParserError("multiple module in one file is not allowed", SourceLocation());
    }
    errors::ParserError Parser::unexpected_token(size_t offset) const noexcept {
        return errors::ParserError(std::format("unexpected token: {}"
        ,                       utils::stringify(peak_tok(offset).ty))
        ,                   peak_tok(offset).pos
        );;
    }

// _tok fuctions

    const Token& Parser::curr_tok() const {
        if(!tokens) throw tokens_nullptr();
        if(pos >= tokens->size()) return tokens->back();
        return (*tokens)[pos];
    }

    const Token& Parser::peak_tok(size_t offset) const {
        if(!tokens) throw tokens_nullptr();
        if(pos + offset >= tokens->size()) return tokens->back();
        return (*tokens)[pos + offset];
    }

    const Token& Parser::next_tok(size_t offset) {
        if(!tokens) throw tokens_nullptr();
        pos += offset;
        if(pos >= tokens->size()) return tokens->back();
        return (*tokens)[pos];
    }

    const Token& Parser::back_tok(size_t offset) {
        if(!tokens) throw tokens_nullptr();
        if(pos - offset < 0) throw std::out_of_range("back_tok: out of range");
        return (*tokens)[pos -= offset];
    }

// some small functions

    bool Parser::is_end() const {
        if(!tokens) throw tokens_nullptr();
        return pos >= tokens->size();
    }

    const Type* Parser::add_type(std::unique_ptr<Type> _type) noexcept {
        type_context.emplace_back(std::move(_type));
        const Type* type = type_context.back().get();
        return type;
    }

// process_ functions

    bool Parser::parse() {
        debug_break();
        while (!is_end()) {
            try { ast.emplace_back(process_token());
            } catch(const std::exception& e) {
                std::println("{}", e.what());
                error = e;
                return true;
            }
        }
        return false;
    }

    ast::StmtPtr Parser::process_token() {
        return process_stmt();
    }
    
    void Parser::process_semicolon() {
        if(curr_tok().ty == TokenType::SEMICOLON) {
            next_tok();
        } else throw expected_semicolon();
    }

    // stmt

    std::unique_ptr<ast::StmtNode> Parser::process_stmt() {
        debug_break();
        if(curr_tok().ty == TokenType::STRUCT)
            throw strcut_is_not_suported();

        if(curr_tok().ty == TokenType::ENUM)
            throw enum_is_not_suported();

        if(curr_tok().ty == TokenType::IMPORT) {
            auto node = process_import_stmt();
            process_semicolon();
            return std::move(node);
        }
        if(curr_tok().ty == TokenType::MODULE) {
            auto node = process_module_decl();
            process_semicolon();
            return std::move(node);
        }
        if(curr_tok().ty == TokenType::LBRACE)
            return process_scope();

        if(curr_tok().ty == TokenType::NAMESPACE)
            return process_namespace_decl();

        if(curr_tok().ty == TokenType::IF)
            throw if_is_not_suported();

        if(curr_tok().ty == TokenType::ELSE)
            throw else_is_not_suported();

        if(curr_tok().ty == TokenType::FOR)
            throw for_is_not_suported();

        if(curr_tok().ty == TokenType::WHILE)
            throw while_is_not_suported();

        if(curr_tok().ty == TokenType::BREAK)
            throw break_is_not_suported();

        if(curr_tok().ty == TokenType::CONTINUE)
            throw continue_is_not_suported();

        if(curr_tok().ty == TokenType::RETURN) {
            auto node = process_return_stmt();
            process_semicolon();
            return std::move(node);
        }

        if(utils::is_declarator(curr_tok().ty) 
        && peak_tok().ty == TokenType::IDENTIFIER)
            return process_declare();
            
        auto node = process_expr();
        process_semicolon();

        return std::move(node);
    }
    
    std::unique_ptr<ast::ImportStmt> Parser::process_import_stmt() {
        debug_break();
        next_tok(); // skip IMPORT TOK
        if(curr_tok().ty != TokenType::IDENTIFIER) throw expected_module_name();
        
        std::string name = curr_tok().sym; next_tok();
        return std::make_unique<ast::ImportStmt>(name);
    }

    std::unique_ptr<ast::ReturnStmt> Parser::process_return_stmt() {
        debug_break();
        next_tok(); // skip RETURN tok

        if(curr_tok().ty == TokenType::SEMICOLON) 
            return std::make_unique<ast::ReturnStmt>();

        return std::make_unique<ast::ReturnStmt>(process_expr());
    }

    std::unique_ptr<ast::DeclModule> Parser::process_module_decl() {
        debug_break();
        if(module_declared) throw multiple_module_decl_in_file();
        next_tok(); // skip MODULE tok
        if(curr_tok().ty != TokenType::IDENTIFIER) throw expected_module_name();
        
        std::string name = curr_tok().sym; next_tok();
        return std::make_unique<ast::DeclModule>(name);
    }

    std::unique_ptr<ast::BlockStmt> Parser::process_scope() {
        debug_break();
        if(curr_tok().ty != TokenType::LBRACE) throw expected_lbrace();
        
        auto block = std::make_unique<ast::BlockStmt>();

        next_tok();
        while(!is_end() && curr_tok().ty != TokenType::RBRACE)
            block->add_tobody(process_token());
        if(is_end() || curr_tok().ty != TokenType::RBRACE) throw expected_rbrace();
        
        next_tok();
        return std::move(block);
    }

    // declare

    std::unique_ptr<ast::DeclStmt> Parser::process_declare() {        
        debug_break();
        if(!(curr_tok().ty == TokenType::EXTERN
        ||   curr_tok().ty == TokenType::CONST)
        &&   peak_tok().ty != TokenType::IDENTIFIER
        )   throw expected_type(1);

        int words{0};
        if(curr_tok().ty == TokenType::EXTERN) {
            ++words;
            next_tok();
        }
        if(curr_tok().ty == TokenType::CONST) {
            ++words;
            next_tok();
        }

        if(peak_tok().ty != TokenType::IDENTIFIER)
            throw expected_identifier(1);

        // function declaration
        if(peak_tok(2).ty == TokenType::LPAREN) {
            if(words > 0) back_tok(words);
            return process_function_decl();
        }

        // variable declaration
        if(peak_tok(2).ty == TokenType::SEMICOLON
        || peak_tok(2).ty == TokenType::ASSIGN) {
            if(words > 0) back_tok(words);
            auto node = process_variable_decl();
            process_semicolon();
            return std::move(node);
        }
        
        std::println("from declare");
        throw unexpected_token(2);
    }

    std::unique_ptr<ast::DeclNamespace> Parser::process_namespace_decl() {
        debug_break();
        if(peak_tok().ty != TokenType::IDENTIFIER) throw expected_namespace_name();
        std::string name = peak_tok().sym;
        next_tok(2);
        
        return std::make_unique<ast::DeclNamespace>(name, process_scope());;
    }

    QualType Parser::process_type() {
        debug_break();
        QualType::Flags flags{QualType::Flags::NONE};
        if(curr_tok().ty == TokenType::CONST) {
            flags |= QualType::Flags::CONST;
            next_tok();
        }
        
        if(curr_tok().ty != TokenType::IDENTIFIER) throw expected_type();
        
        const Type* type = add_type(std::make_unique<Type>(curr_tok().sym));
        next_tok();

        if(curr_tok().ty == TokenType::STAR) { 
            flags | QualType::Flags::POINTER;
            next_tok();
        }
        if(curr_tok().ty == TokenType::AMPERSAND) {
            flags | QualType::Flags::REFERENCE;
            next_tok();
        }

        return QualType(type, flags);
    }

    std::unique_ptr<ast::DeclVariable> Parser::process_variable_decl() {
        debug_break();
        bool is_extern{false};
        if(curr_tok().ty == TokenType::EXTERN) {
            is_extern = true;
            next_tok();
        }

        QualType type = process_type();

        if(curr_tok().ty != TokenType::IDENTIFIER) throw expected_variable_name();

        auto name = curr_tok().sym;
        next_tok();

        if(curr_tok().ty == TokenType::ASSIGN) {
            next_tok(); // skip =
            auto expr = process_expr();
            return std::make_unique<ast::DeclVariable>(name, type, std::move(expr), is_extern);
        }
        return std::make_unique<ast::DeclVariable>(name, type);
    }

    std::unique_ptr<ast::DeclFunction> Parser::process_function_decl() {
        debug_break();
        bool is_extern{false};
        if(curr_tok().ty == TokenType::EXTERN) {
            is_extern = true;
            next_tok();
        }
        
        QualType type = process_type();

        if(curr_tok().ty != TokenType::IDENTIFIER) throw expected_function_name();

        auto name = curr_tok().sym;
        next_tok();

        
        // if(curr_tok().ty != TokenType::LPAREN) throw expected_lparen();
        next_tok();

        std::vector<std::unique_ptr<ast::DeclVariable>> args;
        while(curr_tok().ty != TokenType::RPAREN) {
            args.emplace_back(process_variable_decl());
            if(curr_tok().ty == TokenType::RPAREN) break;
            if(curr_tok().ty != TokenType::COMMA) throw expected_comma();
            next_tok();
        } next_tok(); // skip )


        if(curr_tok().ty == TokenType::SEMICOLON) {
            next_tok();
            return std::make_unique<ast::DeclFunction>(name, type, std::move(args), nullptr);
        }
        
        return std::make_unique<ast::DeclFunction>(name, type, std::move(args), process_scope(), is_extern);
    }

    // expr

    std::unique_ptr<ast::ExprNode> Parser::process_expr() {
        return process_operator();
    }
    
    // TODO: add is_-_op funcs for Token not only for Operator
    std::unique_ptr<ast::ExprNode> Parser::process_operator() {
        return process_assign_expr();
    }
    std::unique_ptr<ast::ExprNode> Parser::process_assign_expr() {
        debug_break();
        auto left = process_logical_expr();
        if(utils::is_operator(curr_tok().ty)) {
            auto op = utils::token_to_op(curr_tok().ty);
            if(utils::is_assign_op(op)) {
                next_tok(); // skip op
                return std::make_unique<ast::BinOpExpr>(op, std::move(left), process_assign_expr(), QualType(nullptr));
            }   
        }
        return std::move(left);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_logical_expr() {
        debug_break();
        auto left = process_compare_expr();
        while(utils::is_operator(curr_tok().ty)
        &&    utils::is_logical_op(utils::token_to_op(curr_tok().ty))) {
            auto op = utils::token_to_op(curr_tok().ty);
            next_tok(); // skip op

            auto right = process_compare_expr();
            left = std::make_unique<ast::BinOpExpr>(op, std::move(left), std::move(right), QualType(nullptr));
        }
        return std::move(left);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_compare_expr() {
        debug_break();
        auto left = process_additive_expr();
        while(utils::is_operator(curr_tok().ty) && utils::is_compare_op(utils::token_to_op(curr_tok().ty))) {
            auto op = utils::token_to_op(curr_tok().ty);
            next_tok(); // skip op

            auto right = process_additive_expr();
            left = std::make_unique<ast::BinOpExpr>(op, std::move(left), std::move(right), QualType(nullptr));
        }
        return std::move(left);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_additive_expr() {
        debug_break();
        auto left = process_multiple_expr();
        while(utils::is_operator(curr_tok().ty) && utils::is_add_op(utils::token_to_op(curr_tok().ty))) {
            auto op = utils::token_to_op(curr_tok().ty);
            next_tok(); // skip op

            auto right = process_multiple_expr();
            left = std::make_unique<ast::BinOpExpr>(op, std::move(left), std::move(right), QualType(nullptr));
        }
        return std::move(left);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_multiple_expr() {
        debug_break();
        auto left = process_unary_expr();
        while(utils::is_operator(curr_tok().ty) && utils::is_mul_op(utils::token_to_op(curr_tok().ty))) {
            auto op = utils::token_to_op(curr_tok().ty);
            next_tok(); // skip op

            auto right = process_unary_expr();
            left = std::make_unique<ast::BinOpExpr>(op, std::move(left), std::move(right), QualType(nullptr));
        }
        
        return std::move(left);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_unary_expr() {
        debug_break();
        if(utils::is_operator(curr_tok().ty)
        && utils::is_prefix_op(utils::token_to_op(curr_tok().ty))) {
            auto op = utils::token_to_op(curr_tok().ty);
            if(!utils::is_unary(op)) throw expected_unary_op();
            next_tok();
            if(utils::is_prefix_op(op)) 
                return std::make_unique<ast::PrefixUnaryOpExpr>(op, process_primary_expr(), QualType(nullptr));
        }
        auto node = process_primary_expr();
        if(utils::is_operator(curr_tok().ty)
        && utils::is_postfix_op(utils::token_to_op(curr_tok().ty))) {
            auto op = utils::token_to_op(curr_tok().ty);
            next_tok();
            return std::make_unique<ast::PostfixUnaryOpExpr>(op, std::move(node), QualType(nullptr));
        }
        return std::move(node);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_primary_expr() {
        debug_break();
        if(curr_tok().ty == TokenType::LPAREN) {
            next_tok();
            auto node = process_expr();
            if(curr_tok().ty != TokenType::RPAREN) throw expected_rparen();
            next_tok(); // skip )
            return std::move(node);
        }
        if(curr_tok().ty == TokenType::IDENTIFIER) 
            return process_name();

        if(utils::is_literal(curr_tok().ty))
            return process_literal();
        
        std::println("from primary_expr");
        throw unexpected_token();
    }

    // names

    std::unique_ptr<ast::ExprNode> Parser::process_name() {
        debug_break();
        if(curr_tok().ty != TokenType::IDENTIFIER) throw expected_identifier();
        if(peak_tok().ty == TokenType::DOUBLECOLON) return process_namespace_expr();
        if(peak_tok().ty == TokenType::LPAREN) return process_function_expr();
        return process_variable_expr();
    }

    std::unique_ptr<ast::NamespaceExpr> Parser::process_namespace_expr() {
        debug_break();
        std::string name = curr_tok().sym;
        next_tok();
        
        if(curr_tok().ty != TokenType::DOUBLECOLON) throw expected_doublecolon();
        next_tok();

        return std::make_unique<ast::NamespaceExpr>(name, process_name(), QualType(nullptr));
    }

    std::unique_ptr<ast::FunctionExpr> Parser::process_function_expr() {
        debug_break();
        // if(curr_tok().ty != TokenType::IDENTIFIER) throw expected_function_name();
        std::string name = curr_tok().sym;
        next_tok();

        if(curr_tok().ty != TokenType::LPAREN) throw expected_lparen();
        next_tok();

        // todo: file end check
        std::vector<ast::ExprPtr> args;
        while(curr_tok().ty != TokenType::RPAREN) {
            args.emplace_back(process_expr());
            if(curr_tok().ty == TokenType::RPAREN) break;
            if(curr_tok().ty != TokenType::COMMA) throw expected_comma();
            next_tok();
        } next_tok();

        return std::make_unique<ast::FunctionExpr>(name, std::move(args), QualType(nullptr));
    }

    std::unique_ptr<ast::VariableExpr> Parser::process_variable_expr() {
        debug_break();
        // if(curr_tok().ty != TokenTcurr_tok().ty != TokenType::RPAREN ype::IDENTIFIER) throw expected_variable_name();
        std::string name = curr_tok().sym;
        next_tok();
        return std::make_unique<ast::VariableExpr>(name, QualType(nullptr));;
    }

    // literals

    std::unique_ptr<ast::LiteralExpr> Parser::process_literal() {
        debug_break();
        switch (curr_tok().ty) {
            case(TokenType::NUMBER): return process_number_literal();
            case(TokenType::STRING): return process_string_literal();
            case(TokenType::TRUE): return process_bool_literal();
            case(TokenType::FALSE): return process_bool_literal();
            std::println("from literals");
            default: throw unexpected_token();
        }
    }

    std::unique_ptr<ast::NumberLiteral> Parser::process_number_literal() {
        debug_break();
        if(curr_tok().ty != TokenType::NUMBER) throw expected_number();
        auto node =std::make_unique<ast::NumberLiteral>(curr_tok().sym, QualType(nullptr));
        next_tok();
        return std::move(node);
    }

    std::unique_ptr<ast::StringLiteral> Parser::process_string_literal() {
        debug_break();
        if(curr_tok().ty != TokenType::STRING) throw expected_string();
        auto node = std::make_unique<ast::StringLiteral>(curr_tok().sym, QualType(nullptr));
        next_tok();
        return std::move(node);
    }

    std::unique_ptr<ast::BoolLiteral> Parser::process_bool_literal() {
        debug_break();
        if(curr_tok().ty != TokenType::TRUE
        && curr_tok().ty != TokenType::FALSE) throw expected_bool();
        auto node = std::make_unique<ast::BoolLiteral>(curr_tok().sym, QualType(nullptr));
        next_tok();
        return std::move(node);
    }
}
