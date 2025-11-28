#include <lang/common.h>
// #define PARSER_DEBUG

#include <format>
#include <string>
#include <lang/utils/frontend_utils.h>
#include <lang/utils/ast_utils.h>
#include <lang/frontend/parser.h>

namespace lang::frontend::parser
{
// public api

    std::vector<std::unique_ptr<ast::BaseNode>> Parser::parse(const std::vector<Token>& _tokens) {
        reset_state();
        tokens = &_tokens;
        std::vector<std::unique_ptr<ast::BaseNode>> ast;
        while(!is_end()) { try {
            ast.emplace_back(process_token());
        } catch(const errors::ParserError& e) {
            success = false;
            logger.error("{}", e.what());
            return {};
        } catch(const std::exception& e) {
            success = false;
            logger.error("error while parsing: {}", e.what());
            return {};
        }} return ast;
    }

    bool Parser::had_errors() const noexcept {
        return !success;
    }

    void Parser::clear_typecontext() noexcept {
        type_context.clear();
    }

// private api
    
    void Parser::reset_state() {
        tokens = nullptr;
        module_declared = false;
        success = true;
        pos = 0;
    }

    void Parser::init_logger() {
        logger.set_name("Parser");
        #ifdef PARSER_DEBUG            
            logger.set_level(utils::Logger::LogLevel::INFO | utils::Logger::LogLevel::WARN | utils::Logger::LogLevel::ERROR);
        #else
            logger.set_level(utils::Logger::LogLevel::ALL);
        #endif 
        // logger.set_level(utils::Logger::LogLevel::ALL); // just for now
    }

    void Parser::breakpoint() {
        #ifdef PARSER_DEBUG
            debug_break();
        #endif
    }

    bool Parser::is_end(size_t n) const {
        if(!tokens) throw tokens_nullptr();
        if(n == 0) throw is_end_with_zero();
        // tokens->size() - 1 bcs of END token
        // but end is not adding by lexer yet :)

        return pos + n >= tokens->size();
    }

    const Type* Parser::add_type(std::unique_ptr<Type> _type) noexcept {
        type_context.emplace_back(std::move(_type));
        const Type* type = type_context.back().get();
        return type;
    }

    bool Parser::match(TokenType tt, size_t offset) const {
        return peek(offset).ty == tt;
    }
    const Token& Parser::peek(size_t offset) const {
        if(is_end(offset + 1)) throw peek_out_of_range();
        return tokens->at(pos + offset);
    }
    Token Parser::advance() {
        auto tok = peek();
        ++pos;
        return std::move(tok);
    }
    void Parser::putback(size_t n) {
        if(pos >= n) pos -= n;
        else throw putback_out_of_range();
    }
    void Parser::skip(size_t n) {
        if(n==0) return;
        pos += n;
    }

// process_ functions

    ast::StmtPtr Parser::process_token() {
        breakpoint(); logger.debug("proccess_token()");
        if(!is_end() && match(TokenType::END)) throw end_reached();
        if(!is_end() && match(TokenType::SEMICOLON)) skip(); // todo: warning: "extra semicolon"
        return process_stmt();
    }
    
    void Parser::process_semicolon() {
        if(is_end()) return;
        if(!is_end() && match(TokenType::SEMICOLON)) skip();
        else throw expected_semicolon();
    }

    // stmt's

    std::unique_ptr<ast::StmtNode> Parser::process_stmt() {
        breakpoint(); logger.debug("proccess_stmt()");
        // modules
        if(!is_end() && match(TokenType::MODULE)) {
            auto node = process_module_decl();
            process_semicolon();
            return std::move(node);
        }
        if(!is_end() && match(TokenType::IMPORT)) {
            auto node = process_import_stmt();
            process_semicolon();
            return std::move(node);
        }
        // types
        if(!is_end() && match(TokenType::STRUCT)) throw strcut_is_not_suported();
        if(!is_end() && match(TokenType::ENUM)) throw enum_is_not_suported();

        // control flow
        if(!is_end() && match(TokenType::IF)) throw if_is_not_suported();
        if(!is_end() && match(TokenType::ELSE)) throw else_is_not_suported();
        if(!is_end() && match(TokenType::FOR)) throw for_is_not_suported();
        if(!is_end() && match(TokenType::WHILE)) throw while_is_not_suported();

        // other stmt's
        if(!is_end() && match(TokenType::LBRACE)) return process_scope();
        if(!is_end() && match(TokenType::BREAK)) throw break_is_not_suported();
        if(!is_end() && match(TokenType::CONTINUE)) throw continue_is_not_suported();
        if(!is_end() && match(TokenType::RETURN)) {
            auto node = process_return_stmt();
            process_semicolon();
            return std::move(node);
        }
        
        // declarations
        if(!is_end() && match(TokenType::NAMESPACE)) return process_namespace_decl();
        if(utils::is_declarator(peek().ty) 
        && match(TokenType::IDENTIFIER, 1)) return process_declare();
            
        // if it not stmt -> try expr :)
        auto node = process_expr();
        process_semicolon();

        return std::move(node);
    }

    // modules stmt's

    std::unique_ptr<ast::DeclModule> Parser::process_module_decl() {
        breakpoint(); logger.debug("proccess_module_decl()");
        if(module_declared) throw multiple_module_decl_in_file();
        skip(); // skip MODULE toks
        if(!is_end() && !match(TokenType::IDENTIFIER)) throw expected_module_name();
        logger.log("parsing module: {}", peek().sym);
        return std::make_unique<ast::DeclModule>(advance().sym);
    }

    std::unique_ptr<ast::ImportStmt> Parser::process_import_stmt() {
        breakpoint(); logger.debug("proccess_import_stmt()");
        skip(); // skip IMPORT TOK
        if(!is_end() && !match(TokenType::IDENTIFIER)) throw expected_module_name();
        return std::make_unique<ast::ImportStmt>(advance().sym);
    }

    // types stmt's - unsupported for now

    // control flow stmt's unsupported for now

    // other stmt's

    std::unique_ptr<ast::BlockStmt> Parser::process_scope() {
        breakpoint(); logger.debug("process_scope()");
        if(!is_end() && !match(TokenType::LBRACE)) throw expected_lbrace();
        
        auto block = std::make_unique<ast::BlockStmt>();

        skip(); // skip '{'
        while(!is_end() && !match(TokenType::RBRACE)) block->add_tobody(process_token());
        if(!is_end() && !match(TokenType::RBRACE)) throw expected_rbrace();
        skip(); // '}'

        return std::move(block);
    }

    std::unique_ptr<ast::ReturnStmt> Parser::process_return_stmt() {
        breakpoint(); logger.debug("proccess_return_stmt()");
        skip(); // skip RETURN tok

        if(!is_end() && match(TokenType::SEMICOLON)) return std::make_unique<ast::ReturnStmt>();
        return std::make_unique<ast::ReturnStmt>(process_expr());
    }

    // declare stmt's

    std::unique_ptr<ast::DeclStmt> Parser::process_declare() {        
        breakpoint(); logger.debug("process_declare()");

        int words{0};
        if(!is_end() && match(TokenType::EXTERN)) {
            ++words;
            skip();
        }
        if(!is_end() && match(TokenType::CONST)) {
            ++words;
            skip();
        }
        if(!is_end() && !match(TokenType::IDENTIFIER)) throw expected_type();
        if(!is_end(1) && !match(TokenType::IDENTIFIER, 1)) throw expected_identifier(1);

        // function declaration
        if(!is_end(2) && match(TokenType::LPAREN, 2)) {
            if(words > 0) putback(words);
            return process_function_decl();
        }

        // variable declaration
        if(!is_end(2) && match(TokenType::SEMICOLON,2)
        || match(TokenType::ASSIGN, 2)) {
            if(words > 0) putback(words);
            auto node = process_variable_decl();
            process_semicolon();
            return std::move(node);
        }
        
        throw unexpected_token(2);
    }

    std::unique_ptr<ast::DeclNamespace> Parser::process_namespace_decl() {
        breakpoint();
        if(peek(1).ty != TokenType::IDENTIFIER) throw expected_namespace_name();
        std::string name = peek(1).sym;
        skip(2);
        
        return std::make_unique<ast::DeclNamespace>(name, process_scope());;
    }

    QualType Parser::process_type() {
        breakpoint(); logger.debug("process_type()");

        QualType::Flags flags{QualType::Flags::NONE};

        if(!is_end() && match(TokenType::CONST)) {
            flags |= QualType::Flags::CONST;
            skip();
        }
        
        // save type name
        if(!is_end() && !match(TokenType::IDENTIFIER)) throw expected_type();
        const Type* type = add_type(std::make_unique<Type>(advance().sym));

        if(!is_end() && match(TokenType::STAR)) { 
            flags | QualType::Flags::POINTER;
            skip();
        }
        if(!is_end() && match(TokenType::AMPERSAND)) {
            flags | QualType::Flags::REFERENCE;
            skip();
        }

        return QualType(type, flags);
    }

    std::unique_ptr<ast::DeclVariable> Parser::process_variable_decl() {
        breakpoint(); logger.debug("process_variable_decl()");

        bool is_extern{false};
        if(!is_end() && match(TokenType::EXTERN)) {
            is_extern = true;
            skip();
        }

        // get type of variable
        QualType type = process_type();

        // save variable name
        if(!is_end() && !match(TokenType::IDENTIFIER)) throw expected_variable_name();
        auto name = advance().sym;

        // if variable has initialization
        if(!is_end() && match(TokenType::ASSIGN)) {
            skip(); // skip =
            auto expr = process_expr();
            return std::make_unique<ast::DeclVariable>(name, type, std::move(expr), is_extern);
        }
        return std::make_unique<ast::DeclVariable>(name, type);
    }

    std::unique_ptr<ast::DeclFunction> Parser::process_function_decl() {
        breakpoint(); logger.debug("process_function_decl()");
        bool is_extern{false};
        if(!is_end() && match(TokenType::EXTERN)) {
            is_extern = true;
            skip();
        }
        
        QualType type = process_type();

        if(!is_end() && !match(TokenType::IDENTIFIER)) throw expected_function_name();

        auto name = advance().sym;

        
        // don't check bcs process_decl want call process_function_decl without paren('(')
        // if(!is_end() && !match(TokenType::LPAREN)) throw expected_lparen();
        skip(); // skip '('

        std::vector<std::unique_ptr<ast::DeclVariable>> args;
        while(! is_end() && !match(TokenType::RPAREN)) {
            args.emplace_back(process_variable_decl());
            if(!is_end() && match(TokenType::RPAREN)) break;
            if(!is_end() && !match(TokenType::COMMA)) throw expected_comma();
            skip(); // skip ','
        } if(!is_end() && !match(TokenType::RPAREN)) throw expected_rparen();
        skip(); // skip ')'

        // if forward declaration
        if(!is_end() && match(TokenType::SEMICOLON)) { skip(); // skip ';'
            return std::make_unique<ast::DeclFunction>(name, type, std::move(args), nullptr);
        } return std::make_unique<ast::DeclFunction>(name, type, std::move(args), process_scope(), is_extern);
    }

    // expr's

    std::unique_ptr<ast::ExprNode> Parser::process_expr() {
        return process_operator();
    }
    
    // TODO: add is_-_op funcs for TokenType not only for Operator
    std::unique_ptr<ast::ExprNode> Parser::process_operator() {
        return process_assign_expr();
    }
    std::unique_ptr<ast::ExprNode> Parser::process_assign_expr() {
        breakpoint(); logger.debug("process_assign_expr()");
        auto left = process_logical_expr();
        if(utils::is_operator(peek().ty)) {
            auto op = utils::token_to_op(peek().ty);
            if(utils::is_assign_op(op)) { skip(); // skip op
                return std::make_unique<ast::BinOpExpr>(op, std::move(left), process_assign_expr(), QualType(nullptr));
            }   
        } return std::move(left);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_logical_expr() {
        breakpoint(); logger.debug("process_logical_expr()");
        auto left = process_compare_expr();
        while(utils::is_operator(peek().ty)) {
            auto op = utils::token_to_op(peek().ty);
            if(utils::is_logical_op(op)) { skip(); // skip op
                left = std::make_unique<ast::BinOpExpr>(op, std::move(left), process_compare_expr(), QualType(nullptr));
            } else break;
        } return std::move(left);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_compare_expr() {
        breakpoint(); logger.debug("process_compare_expr()");
        auto left = process_additive_expr();
        while(utils::is_operator(peek().ty)) {
            auto op = utils::token_to_op(peek().ty);
            if(utils::is_compare_op(op)) { skip(); // skip op
                left = std::make_unique<ast::BinOpExpr>(op, std::move(left), process_additive_expr(), QualType(nullptr));
            } else break;
        } return std::move(left);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_additive_expr() {
        breakpoint(); logger.debug("process_additive_expr()");
        auto left = process_multiple_expr();
        while(utils::is_operator(peek().ty)) {
            auto op = utils::token_to_op(peek().ty);
            if(utils::is_add_op(op)) { skip(); // skip op
                left = std::make_unique<ast::BinOpExpr>(op, std::move(left), process_multiple_expr(), QualType(nullptr));
            } else break;
        } return std::move(left);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_multiple_expr() {
        breakpoint(); logger.debug("process_multiple_expr()");
        auto left = process_unary_expr();
        while(utils::is_operator(peek().ty)) {
            auto op = utils::token_to_op(peek().ty);
            if(utils::is_mul_op(op)) { skip(); // skip op
                left = std::make_unique<ast::BinOpExpr>(op, std::move(left), process_unary_expr(), QualType(nullptr));
            } else break;
        } return std::move(left);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_unary_expr() {
        breakpoint(); logger.debug("process_unary_expr()");
        
        // PREfix op
        if(utils::is_operator(peek().ty)) {
            auto op = utils::token_to_op(peek().ty);
            if(utils::is_prefix_op(op)) { skip(); // skip op
                return std::make_unique<ast::PrefixUnaryOpExpr>(op, process_primary_expr(), QualType(nullptr));
            } throw expected_unary_op(); // bcs only prefix operator can be here
        }

        auto node = process_primary_expr();

        // POSTfix op
        if(utils::is_operator(peek().ty)) {
            auto op = utils::token_to_op(peek().ty);
            if(utils::is_postfix_op(op)) return std::make_unique<ast::PostfixUnaryOpExpr>(utils::token_to_op(advance().ty), std::move(node), QualType(nullptr));
            // throw expected_postfix_op(); conflict with binary operators
        } return std::move(node);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_primary_expr() {
        breakpoint(); logger.debug("process_primary_expr()");
        if(!is_end() && match(TokenType::LPAREN)) { skip(); // skip '('
            auto node = process_expr();
            if(!is_end() && !match(TokenType::RPAREN)) throw expected_rparen();
            skip(); // skip ')'
            return std::move(node);
        }
        if(!is_end() && match(TokenType::IDENTIFIER)) return process_name();
        if(utils::is_literal(peek().ty)) return process_literal();
        
        throw unexpected_token();
    }

    // names

    std::unique_ptr<ast::ExprNode> Parser::process_name() {
        breakpoint(); logger.debug("process_name()");
        if(!is_end() && !match(TokenType::IDENTIFIER)) throw expected_identifier();
        if(!is_end(1) && match(TokenType::DOUBLECOLON, 1)) return process_namespace_expr();
        if(!is_end(1) && match(TokenType::LPAREN, 1)) return process_function_expr();
        return process_variable_expr();
    }

    std::unique_ptr<ast::NamespaceExpr> Parser::process_namespace_expr() {
        breakpoint(); logger.debug("process_namespace_expr()");
        std::string name = advance().sym;
        
        // useless - already checked by process_name() to call this
        // if(!is_end() && !match(TokenType::DOUBLECOLON)) throw expected_doublecolon();
        skip(); // skip '::'

        return std::make_unique<ast::NamespaceExpr>(name, process_name(), QualType(nullptr));
    }

    std::unique_ptr<ast::FunctionExpr> Parser::process_function_expr() {
        breakpoint(); logger.debug("process_function_expr()");
        // if(!is_end() && !match(TokenType::IDENTIFIER) throw expected_function_name();
        std::string name = advance().sym;

        // useless - already checked by process_name() to call this
        // if(!is_end() && !match(TokenType::LPAREN)) throw expected_lparen();
        skip(); // skip '('

        // todo: file end check
        std::vector<ast::ExprPtr> args;
        while(!is_end() && !match(TokenType::RPAREN)) {
            args.emplace_back(process_expr());
            if(!is_end() && match(TokenType::RPAREN)) break;
            if(!is_end() && !match(TokenType::COMMA)) throw expected_comma();
            skip(); // skip comma
        } if(is_end()) throw expected_rparen();
        skip(); // skip ')'
        return std::make_unique<ast::FunctionExpr>(name, std::move(args), QualType(nullptr));
    }

    std::unique_ptr<ast::VariableExpr> Parser::process_variable_expr() {
        breakpoint(); logger.debug("process_variable_expr()");
        // if(!match(TokenT!match(TokenType::RPAREN ype::IDENTIFIER) throw expected_variable_name();
        std::string name = advance().sym;
        return std::make_unique<ast::VariableExpr>(name, QualType(nullptr));;
    }

    // literals

    std::unique_ptr<ast::LiteralExpr> Parser::process_literal() {
        breakpoint(); logger.debug("process_literal()");
        switch (peek().ty) {
            case(TokenType::NUMBER): return process_number_literal();
            case(TokenType::STRING): return process_string_literal();
            case(TokenType::TRUE):   return process_bool_literal();
            case(TokenType::FALSE):  return process_bool_literal();
            default: throw unexpected_token();
        }
    }

    std::unique_ptr<ast::NumberLiteral> Parser::process_number_literal() {
        breakpoint(); logger.debug("process_number_literal()");
        // useless - already checked by process_literal() to call this
        // if(!is_end() && !match(TokenType::NUMBER)) throw expected_number();
        return std::make_unique<ast::NumberLiteral>(advance().sym, QualType(nullptr));
    }

    std::unique_ptr<ast::StringLiteral> Parser::process_string_literal() {
        breakpoint(); logger.debug("process_string_literal()");
        // useless - already checked by process_literal() to call this
        // if(!is_end() && !match(TokenType::STRING)) throw expected_string();
        return std::make_unique<ast::StringLiteral>(advance().sym, QualType(nullptr));
    }

    std::unique_ptr<ast::BoolLiteral> Parser::process_bool_literal() {
        breakpoint(); logger.debug("process_bool_literal()");
        // useless - already checked by process_literal() to call this
        // if(!is_end() && !match(TokenType::TRUE) || !match(TokenType::FALSE)) throw expected_bool();
        return std::make_unique<ast::BoolLiteral>(advance().sym, QualType(nullptr));
    }

// errors creating
    
    errors::ParserError Parser::tokens_nullptr(size_t offset) const noexcept {
        return errors::ParserError("tokens = nullptr", SourceLocation());
    }
    errors::ParserError Parser::is_end_with_zero(size_t offset) const noexcept {
        return errors::ParserError("is_end() called with 0", SourceLocation());
    }
    errors::ParserError Parser::peek_out_of_range(size_t offset) const noexcept {
        return errors::ParserError("peek(): out of range", SourceLocation());
    }
    errors::ParserError Parser::putback_out_of_range(size_t offset) const noexcept {
        return errors::ParserError("putback(): out of range", SourceLocation());
    }
    errors::ParserError Parser::end_reached(size_t offset) const noexcept {
        return errors::ParserError("end reached", SourceLocation());
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

    errors::ParserError Parser::expected_semicolon(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected ';', got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_lbrace(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected '{{', got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_rbrace(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected '}}', got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_lparen(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected '(', got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_rparen(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected ')', got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_comma(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected ',', got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_doublecolon(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected '::', got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_unary_op(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected unary operator, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_postfix_op(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected postfix operator, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_identifier(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected identifier, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_function_name(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected function name, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_variable_name(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected variable name, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_namespace_name(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected namespace name, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_module_name(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected module name, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_type(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected type, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_number(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected number, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_string(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected string, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::expected_bool(size_t offset) const noexcept {
        return errors::ParserError(std::format("expected bool, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
    }
    errors::ParserError Parser::unexpected_token(size_t offset) const noexcept {
        return errors::ParserError(std::format("unexpected token: {}"
        ,                       utils::stringify(peek(offset).ty))
        ,                   peek(offset).pos
        );;
    }
}
