#include <format>
#include <lang/utils/syntax_utils.h>
#include <lang/utils/ast_utils.h>
#include <lang/syntax/parser.h>

// #define PARSER_DEBUG

namespace lang::syntax::parser
{
// public api

    SyntaxContainer Parser::parse(const std::vector<Token>& _tokens) {
        reset_state();
        tokens = &_tokens;
        std::vector<std::unique_ptr<ast::BaseNode>> ast;
        while(!is_end()) { try {
            ast.emplace_back(process_token());
        } catch(const diagnostic::ParserError& e) {
            success = false;
            logger.error("{}", e.what());
            return {};
        } catch(const std::exception& e) {
            success = false;
            logger.error("inter error while parsing: {}", e.what());
            return {};
        }} return SyntaxContainer{
            .ast = std::move(ast),
            .types_context = std::move(types_context),
            .extern_list = std::move(extern_list)
        };
    }

    bool Parser::had_errors() const noexcept {
        return !success;
    }

// private api

    void Parser::init() {
        init_logger();
    }

    void Parser::init_logger() {
        logger.set_name("Parser");
        #ifdef PARSER_DEBUG
            logger.set_level(common::utils::Logger::LogLevel::ALL);
        #else
            logger.set_level(common::utils::Logger::LogLevel::INFO | common::utils::Logger::LogLevel::WARN | common::utils::Logger::LogLevel::ERROR);
        #endif
        // logger.set_level(common::utils::Logger::LogLevel::ALL); // just for now
    }
    
    void Parser::reset_state() {
        tokens = nullptr;
        success = true;
        pos = 0;

        types_context.clear();
        export_list.clear();
        extern_list.clear();
    }

    void Parser::breakpoint() {
        #ifdef PARSER_DEBUG
            common::debug_break();
        #endif
    }

    bool Parser::is_end(size_t n) const {
        if(!tokens) throw tokens_nullptr();
        if(n == 0) throw is_end_with_zero();
        // fix that shit
        return pos + (n - 1) >= tokens->size();
    }

    void Parser::save_type_to_context(ast::DeclStmt* node, std::unique_ptr<AbstractType> type) {
        types_context[node] = std::move(type);
    }
    void Parser::add_to_extern_list(ast::DeclStmt* node) {
        extern_list.emplace(node);
    }
    void Parser::add_to_export_list(ast::DeclStmt* node) {
        export_list.emplace(node);
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

    size_t Parser::save_pos() {
        return pos;
    }
    void Parser::load_pos(size_t new_pos) {
        pos = new_pos;
    }

// process_ functions

    ast::StmtPtr Parser::process_token() {
        breakpoint(); logger.debug("proccess_token()");
        if(is_end()) throw end_reached();
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
        // import
        if(!is_end() && match(TokenType::IMPORT)) {
            auto pos = peek().pos;
            auto node = process_import_stmt();
            if(!is_end()) pos.merge(peek().pos); // merging semicolon
            process_semicolon();
            node->set_source_pos(pos);
            return std::move(node);
        }
        // export import
        if(!is_end(2) 
        && match(TokenType::EXPORT)
        && match(TokenType::IMPORT, 1)) { skip(); // skip export
            auto export_pos = peek().pos;
            auto import_pos = peek().pos;
            auto node = process_import_stmt();
            if(!is_end()) import_pos.merge(peek().pos);
            process_semicolon();
            node->set_source_pos(import_pos);
            extern_list.emplace(node.get());
            return std::move(node);
        }
        
        // types
        if(!is_end() && match(TokenType::STRUCT)) throw strcut_is_not_suported();
        if(!is_end() && match(TokenType::ENUM)) throw enum_is_not_suported();

        // control flow
        if(!is_end() && match(TokenType::IF)) return process_if_stmt();
        if(!is_end() && match(TokenType::ELSE)) return process_else_stmt();
        if(!is_end() && match(TokenType::FOR)) return process_for_stmt();
        if(!is_end() && match(TokenType::WHILE)) return process_while_stmt();

        // other stmts
        if(!is_end() && match(TokenType::LBRACE)) return process_scope();
        if(!is_end() && match(TokenType::BREAK)) throw break_is_not_suported();
        if(!is_end() && match(TokenType::CONTINUE)) throw continue_is_not_suported();
        if(!is_end() && match(TokenType::RETURN)) {
            auto loc = peek().pos;
            auto node = process_return_stmt();
            if(!is_end()) loc.merge(peek().pos);
            process_semicolon();
            return std::move(node);
        }

        // declarations        
        if(!is_end() && look_like_declare()) return process_declare();

        // if it not stmt -> try expr :)
        auto node = process_expr();
        process_semicolon();

        return std::move(node);
    }

    // modules stmts
    std::unique_ptr<ast::ImportStmt> Parser::process_import_stmt() {
        breakpoint(); logger.debug("proccess_import_stmt()");
        skip(); // skip IMPORT TOK

        bool is_relative{false};
        if(!is_end() && match(TokenType::DOT)) { skip(); // skip .
            is_relative = true;
        }


        std::vector<std::string> path;
        if(!is_end() && !match(TokenType::IDENTIFIER)) throw expected_module_name();
        auto path_loc = peek().pos;

        path.emplace_back(advance().sym);
        while(!is_end() && match(TokenType::DOUBLECOLON)) { skip(); // skip ::
            if(!is_end() && !match(TokenType::IDENTIFIER)) throw expected_submodule_name();
            path_loc.merge(peek().pos);
            path.emplace_back(advance().sym);
        }   

        return std::make_unique<ast::ImportStmt>(SymbolPath{
            .path = std::move(path)
        }, is_relative, path_loc);
    }

    // types stmts - unsupported for now

    // control flow stmts

    std::unique_ptr<ast::IfStmt> Parser::process_if_stmt() {
        breakpoint(); logger.debug("proccess_if_stmt()");
        auto loc = peek().pos;
        auto word_loc = peek().pos;
        skip(); // skip IF tok

        // condition
        if(!(!is_end() && match(TokenType::LPAREN))) throw expected_lparen();
        skip(); // skip '('
        auto cond = process_expr();

        if(!(!is_end() && match(TokenType::RPAREN))) throw expected_rparen();
        skip(); // skip ')'

        // body
        ast::StmtPtr body{nullptr};
        if(!is_end() && match(TokenType::LBRACE)) body = process_scope();
        else body = process_token();

        if(body->get_source_pos() != common::SourceLocation()) loc.merge(body->get_source_pos());

        return std::make_unique<ast::IfStmt>(
            std::move(cond), 
            std::move(body), 
            std::move(word_loc),
            std::move(loc)
        );
    }

    std::unique_ptr<ast::ElseStmt> Parser::process_else_stmt() {
        breakpoint(); logger.debug("proccess_else_stmt()");
        auto loc = peek().pos;
        auto word_loc = peek().pos;
        skip(); // skip ELSE tok

        // body
        ast::StmtPtr body{nullptr};
        if(!is_end() && match(TokenType::LBRACE)) body = process_scope();
        else body = process_token();

        if(body->get_source_pos() != common::SourceLocation()) loc.merge(body->get_source_pos());

        return std::make_unique<ast::ElseStmt>(
            std::move(body),
            std::move(word_loc),
            std::move(loc)
        );
    }

    std::unique_ptr<ast::ForStmt> Parser::process_for_stmt() {
        breakpoint(); logger.debug("proccess_for_stmt()");
        auto loc = peek().pos;
        auto word_loc = peek().pos;
        skip(); // skip IF tok

        if(!(!is_end() && match(TokenType::LPAREN))) throw expected_lparen();
        skip(); // skip '('

        // decl
        ast::StmtPtr decl{nullptr};
        if(!is_end() && !match(TokenType::SEMICOLON)) decl = process_variable_decl();
        process_semicolon();

        // condition
        ast::ExprPtr cond{nullptr};
        if(!is_end() && !match(TokenType::SEMICOLON)) cond = process_expr();
        process_semicolon();

        // incr/decr
        ast::StmtPtr incr = process_expr();

        if(!(!is_end() && match(TokenType::RPAREN))) throw expected_rparen();
        skip(); // skip ')'

        // body
        ast::StmtPtr body{nullptr};
        if(!is_end() && match(TokenType::LBRACE)) body = process_scope();
        else body = process_token();

        if(body->get_source_pos() != common::SourceLocation()) loc.merge(body->get_source_pos());

        return std::make_unique<ast::ForStmt>(
            std::move(decl),
            std::move(cond),
            std::move(incr),
            std::move(body),
            std::move(word_loc),
            std::move(loc)
        );
    }

    std::unique_ptr<ast::WhileStmt> Parser::process_while_stmt() {
        breakpoint(); logger.debug("proccess_while_stmt()");
        auto loc = peek().pos;
        auto word_loc = peek().pos;
        skip(); // skip IF tok

        // condition
        if(!(!is_end() && match(TokenType::LPAREN))) throw expected_lparen();
        skip(); // skip '('
        auto cond = process_expr();

        if(!(!is_end() && match(TokenType::RPAREN))) throw expected_rparen();
        skip(); // skip ')'

        // body
        ast::StmtPtr body;
        if(!is_end() && match(TokenType::LBRACE)) body = process_scope();
        else body = process_token();

        if(body->get_source_pos() != common::SourceLocation()) loc.merge(body->get_source_pos());
        return std::make_unique<ast::WhileStmt>(
            std::move(cond),
            std::move(body),
            std::move(word_loc),
            std::move(loc)
        );
    }

    // other stmts

    std::unique_ptr<ast::BlockStmt> Parser::process_scope() {
        breakpoint(); logger.debug("process_scope()");
        if(!is_end() && !match(TokenType::LBRACE)) throw expected_lbrace();
        
        auto block = std::make_unique<ast::BlockStmt>();

        auto loc = advance().pos;  // skip '{'
        while(!is_end() && !match(TokenType::RBRACE)) block->add_tobody(process_token());
        if(!is_end() && !match(TokenType::RBRACE)) throw expected_rbrace();
        loc.merge(advance().pos); // '}'

        block->set_source_pos(loc);
        return std::move(block);
    }

    std::unique_ptr<ast::ReturnStmt> Parser::process_return_stmt() {
        breakpoint(); logger.debug("proccess_return_stmt()");
        skip(); // skip RETURN tok

        if(!is_end() && match(TokenType::SEMICOLON)) return std::make_unique<ast::ReturnStmt>();
        return std::make_unique<ast::ReturnStmt>(process_expr());
    }

    std::unique_ptr<ast::BreakStmt> Parser::process_break_stmt() {
        breakpoint(); logger.debug("proccess_break_stmt()");
        skip(); // skip BREAK tok
        return std::make_unique<ast::BreakStmt>();
    }

    std::unique_ptr<ast::ContinueStmt> Parser::process_continue_stmt() {
        breakpoint(); logger.debug("proccess_continue_stmt()");
        skip(); // skip BREAK tok
        return std::make_unique<ast::ContinueStmt>();
    }

    // declare stmts

    bool Parser::look_like_declare() {
        if(!is_end() && match(TokenType::EXPORT)) {
            return true;
        }
        if(!is_end() && match(TokenType::EXTERN)) {
            return true;
        }

        int words{0};
        while(!is_end() && (
            match(TokenType::AMPERSAND)
        ||  match(TokenType::STAR)
        ||  match(TokenType::CONST)
        ||  match(TokenType::FN))) {
            if(match(TokenType::FN)
            || match(TokenType::CONST)) {
                putback(words);
                return true;
            } skip(); ++words;
        }

        // type name
        if(!is_end(1) && !match(TokenType::IDENTIFIER)) {
            putback(words); return false;
        }
        
        //  symbol name
        if(!is_end(2) && !match(TokenType::IDENTIFIER, 1)) {
            putback(words); return false;
        }

        // function declaration
        if(!is_end(3) && match(TokenType::LPAREN, 2)) {
            putback(words); return true;
        }

        // variable declaration
        if(!is_end(3) && (
            match(TokenType::SEMICOLON,2)
        ||  match(TokenType::ASSIGN, 2)
        ||  (!is_end(4)
        &&  match(TokenType::LARROW, 2)
        // check for stack also, so <- will not used as =
        &&  match(TokenType::STACK, 3)))) {
            putback(words); return true;
        } return false;
    }

    std::unique_ptr<ast::DeclStmt> Parser::process_declare() {        
        breakpoint(); logger.debug("process_declare()");

        bool is_export{false};
        if(!is_end() && match(TokenType::EXPORT)) {
            is_export = true;
            skip();
        }

        bool is_extern{false};
        if(!is_end() && match(TokenType::EXTERN)) {
            is_extern = true;
            skip();
        }

        auto pos = save_pos();
        // will it cause memory leak? - i think no,
        // but not 100% sure that will not cause problems
        process_type(); // skip the type
        
        //  symbol name
        if(!is_end(1) && !match(TokenType::IDENTIFIER)) throw expected_identifier(1);

        // function declaration
        if(!is_end(2) && match(TokenType::LPAREN, 1)) {
            load_pos(pos);
            auto node = process_function_decl();
            if(is_export) add_to_export_list(node.get());
            if(is_extern) add_to_extern_list(node.get());
            return std::move(node);
        }

        // variable declaration
        if(!is_end(2) && (
            match(TokenType::SEMICOLON,1)
        ||  match(TokenType::ASSIGN, 1)
        ||  (!is_end(3)
        &&  match(TokenType::LARROW, 1)
        // check for stack also, so <- will not used as =
        &&  match(TokenType::STACK, 2)))) {
            load_pos(pos);
            auto node = process_variable_decl();
            if(is_export) add_to_export_list(node.get());
            if(is_extern) add_to_extern_list(node.get());
            process_semicolon();
            return std::move(node);
        } throw unexpected_token(2);
    }

    std::unique_ptr<AbstractType> Parser::process_type() {
        breakpoint(); logger.debug("process_type()");

        auto loc = peek().pos;
        if(!is_end() && match(TokenType::AMPERSAND)) { skip();
            return std::make_unique<WrapperType>(
                WrapperType::WrapperKind::REFERENCE,
                process_type(),
                loc
            );
        }

        if(!is_end() && match(TokenType::STAR)) { skip();
            return std::make_unique<WrapperType>(
                WrapperType::WrapperKind::POINTER,
                process_type(),
                loc
            );
        }

        if(!is_end() && match(TokenType::CONST)) { skip();
            return std::make_unique<WrapperType>(
                WrapperType::WrapperKind::CONST,
                process_type(),
                loc
            );
        }

        if(!is_end() && match(TokenType::MUTABLE)) { skip();
            return std::make_unique<WrapperType>(
                WrapperType::WrapperKind::MUTABLE,
                process_type(),
                loc
            );
        }
        
        if(!is_end() && match(TokenType::IDENTIFIER)) {
            return std::make_unique<UnresolvedType>(advance().sym, loc);
        }

        /*
            - `*fn(int, int) x;` pointer to function `fn(int, int) -> void`
            - `*fn(int, int) -> int x;` pointer to function `fn(int, int) -> int`
        */
        if(!is_end() && match(TokenType::FN)) { skip(); // skip fn keyword
            if(!is_end() && !match(TokenType::LPAREN)) throw expected_lparen();
            skip(); // skip '('

            std::vector<std::unique_ptr<AbstractType>> args_types;
            std::unique_ptr<AbstractType> return_type = std::make_unique<UnresolvedType>("void");

            while(!is_end() && !match(TokenType::RPAREN)) {
                args_types.emplace_back(process_type());
                if(!is_end() && match(TokenType::RPAREN)) break;
                if(!is_end() && !match(TokenType::COMMA)) throw expected_comma();
                skip(); // skip ','
            } 
            
            if(!is_end() && !match(TokenType::RPAREN)) throw expected_rparen();
            skip(); // skip ')'

            if(!is_end() && match(TokenType::RARROW)) { skip(); // skip "->"
                return_type = process_type();
            } 
            
            return std::make_unique<FunctionType>(
                std::move(args_types),
                std::move(return_type),
                loc
            );
        } throw unexpected_token();
    }

    std::unique_ptr<ast::DeclVariable> Parser::process_variable_decl() {
        breakpoint(); logger.debug("process_variable_decl()");
        auto loc = peek().pos;

        // get type of variable
        auto type = process_type();

        // save variable name
        if(!is_end() && !match(TokenType::IDENTIFIER)) throw expected_variable_name();
        auto name_loc = peek().pos;
        auto name = advance().sym;

        // if variable has initialization
        if(!is_end(2) && (
            match(TokenType::ASSIGN)
        ||  (match(TokenType::LARROW)
        // check for stack also, so <- will not used as =
        &&  match(TokenType::STACK, 1)))) {skip(); // skip = or <-
            auto init_expr = process_expr();
            if(init_expr->get_source_pos() != common::SourceLocation()) loc.merge(init_expr->get_source_pos());
            auto node = std::make_unique<ast::DeclVariable>(
                name,
                name_loc,
                std::move(init_expr),
                loc
            );
            save_type_to_context(node.get(), std::move(type));
            return std::move(node);
        }

        // can merge just to name loc, here no init expr 
        loc.merge(name_loc);
        auto node = std::make_unique<ast::DeclVariable>(
            name,
            name_loc,
            nullptr,
            loc
        );
        save_type_to_context(node.get(), std::move(type));
        return std::move(node);
    }

    std::unique_ptr<ast::DeclFunction> Parser::process_function_decl() {
        breakpoint(); logger.debug("process_function_decl()");
        auto loc = peek().pos;
        
        auto type = process_type();

        // save name
        if(!is_end() && !match(TokenType::IDENTIFIER)) throw expected_function_name();
        auto name_loc = peek().pos;
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
        if(!is_end() && match(TokenType::SEMICOLON)) { 
            loc.merge(advance().pos); // skip ';'
            auto node = std::make_unique<ast::DeclFunction>(
                name, 
                std::move(args), 
                std::move(name_loc),
                nullptr,
                std::move(loc)
            );
            save_type_to_context(node.get(), std::move(type));
            return std::move(node);
        } 
        
        auto body = process_scope();
        if(body->get_source_pos() != common::SourceLocation()) loc.merge(body->get_source_pos());
        auto node = std::make_unique<ast::DeclFunction>(
            name,
            std::move(args),
            std::move(name_loc),
            std::move(body),
            std::move(loc)
        );
        save_type_to_context(node.get(), std::move(type));
        return std::move(node);
    }

    // exprs

    // TODO:
    // can process location for ALL expr just here 
    std::unique_ptr<ast::ExprNode> Parser::process_expr() {
        breakpoint(); logger.debug("process_expr");
        if(!is_end() && match(TokenType::STACK)) return process_stackalloc_expr();
        return process_operator();
    }

    std::unique_ptr<ast::StackAllocExpr> Parser::process_stackalloc_expr() {
        breakpoint(); logger.debug("process_stackalloc_expr()");
        skip(); // skip stack keyword
        if(!is_end() && !match(TokenType::LBRACKET)) throw expected_lbracket();

        std::vector<size_t> dimensions;
        std::vector<common::SourceLocation> locs;
        while(!is_end() && match(TokenType::LBRACKET)) { 
            auto loc = advance().pos; // skip [
            if(!is_end() && !match(TokenType::NUMBER)) throw expected_number();
            dimensions.emplace_back(std::stoull(advance().sym));
            if(!is_end() && !match(TokenType::RBRACKET)) throw expected_rbracket();
            loc.merge(advance().pos); // skip ]
            locs.emplace_back(loc);
        }

        // check for initialization
        if(!is_end() && match(TokenType::ASSIGN)) throw stack_initialization_not_supported();

        return std::make_unique<ast::StackAllocExpr>(
            std::move(dimensions),
            std::move(locs)
        );
    }
    
    // TODO: add is_-_op funcs for TokenType not only for Operator
    std::unique_ptr<ast::ExprNode> Parser::process_operator() {
        return process_assign_expr();
    }
    std::unique_ptr<ast::ExprNode> Parser::process_assign_expr() {
        breakpoint(); logger.debug("process_assign_expr()");
        auto left = process_logical_expr();
        if(!is_end(1) && utils::is_operator(peek().ty)) {
            auto op = utils::token_to_op(peek().ty);
            if(utils::is_assign_op(op)) { auto op_loc = advance().pos; // skip op
                return std::make_unique<ast::BinOpExpr>(
                    op,
                    std::move(left),
                    process_assign_expr(),
                    std::move(op_loc)
                );
            }   
        } return std::move(left);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_logical_expr() {
        breakpoint(); logger.debug("process_logical_expr()");
        auto left = process_compare_expr();
        while(!is_end(1) &&  utils::is_operator(peek().ty)) {
            auto op = utils::token_to_op(peek().ty);
            if(utils::is_logical_op(op)) { auto op_loc = advance().pos; // skip op
                left = std::make_unique<ast::BinOpExpr>(
                    op,
                    std::move(left),
                    process_compare_expr(),
                    std::move(op_loc)
                );
            } else break;
        } return std::move(left);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_compare_expr() {
        breakpoint(); logger.debug("process_compare_expr()");
        auto left = process_additive_expr();
        while(!is_end(1) && utils::is_operator(peek().ty)) {
            auto op = utils::token_to_op(peek().ty);
            if(utils::is_compare_op(op)) { auto op_loc = advance().pos; // skip op
                left = std::make_unique<ast::BinOpExpr>(op,
                    std::move(left),
                    process_additive_expr(),
                    std::move(op_loc)
                );
            } else break;
        } return std::move(left);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_additive_expr() {
        breakpoint(); logger.debug("process_additive_expr()");
        auto left = process_multiple_expr();
        while(!is_end(1) && utils::is_operator(peek().ty)) {
            auto op = utils::token_to_op(peek().ty);
            if(utils::is_add_op(op)) { auto op_loc = advance().pos; // skip op
                left = std::make_unique<ast::BinOpExpr>(
                    op,
                    std::move(left),
                    process_multiple_expr(),
                    std::move(op_loc)
                );
            } else break;
        } return std::move(left);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_multiple_expr() {
        breakpoint(); logger.debug("process_multiple_expr()");
        auto left = process_unary_expr();
        while(!is_end(1) && utils::is_operator(peek().ty)) {
            auto op = utils::token_to_op(peek().ty);
            if(utils::is_mul_op(op)) { auto op_loc = advance().pos; // skip op
                return std::make_unique<ast::BinOpExpr>(op,
                    std::move(left),
                    process_unary_expr(),
                    std::move(op_loc)
                );
            } else break;
        } return std::move(left);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_unary_expr() {
        breakpoint(); logger.debug("process_unary_expr()");
        
        // PREfix op
        if(!is_end(1) && utils::is_operator(peek().ty)) {
            auto op = utils::token_to_op(peek().ty);
            if(utils::is_prefix_op(op)) { auto op_loc = advance().pos; // skip op
                return std::make_unique<ast::PrefixUnaryOpExpr>(
                    op,
                    process_primary_expr(),
                    std::move(op_loc)
                );
            } throw expected_unary_op(); // bcs only prefix operator can be here
        }

        auto node = process_primary_expr();

        // POSTfix op
        if(!is_end(1) && utils::is_operator(peek().ty)) {
            auto op_loc = peek().pos;
            auto op = utils::token_to_op(peek().ty);
            if(utils::is_postfix_op(op)) return std::make_unique<ast::PostfixUnaryOpExpr>(
                utils::token_to_op(advance().ty),
                std::move(node),
                std::move(op_loc)
            ); // throw expected_postfix_op(); conflict with binary operators
        } return std::move(node);
    }
    std::unique_ptr<ast::ExprNode> Parser::process_primary_expr() {
        breakpoint(); logger.debug("process_primary_expr()");
        if(!is_end() && match(TokenType::LPAREN)) {
            auto node_loc = advance().pos; // skip '('
            auto node = process_expr();
            if(!is_end() && !match(TokenType::RPAREN)) throw expected_rparen();
            node_loc.merge(advance().pos); // skip ')'
            node->set_source_pos(std::move(node_loc));
            return std::move(node);
        }
        if(!is_end() && match(TokenType::IDENTIFIER)) return process_name();
        if(utils::is_literal(peek().ty)) return process_literal();
        
        throw unexpected_token();
    }

    // names

    std::unique_ptr<ast::ExprNode> Parser::process_name() {
        breakpoint(); logger.debug("process_name()");
        if(!is_end(1) && !match(TokenType::IDENTIFIER)) throw expected_identifier();
        if(!is_end(2) && match(TokenType::DOUBLECOLON, 1)) return process_symbol_path();
        if(!is_end(2) && match(TokenType::LPAREN, 1)) return process_function_expr();
        return process_variable_expr();
    }

    std::unique_ptr<ast::SymbolPathExpr> Parser::process_symbol_path() {
        breakpoint(); logger.debug("process_namespace_expr()");

        auto name_loc = peek().pos;
        std::string name = advance().sym;
        
        // useless - already checked by process_name() to call this
        // if(!is_end() && !match(TokenType::DOUBLECOLON)) throw expected_doublecolon();
        skip(); // skip '::'

        return std::make_unique<ast::SymbolPathExpr>(
            name,
            process_name(),
            std::move(name_loc)
        );
    }

    std::unique_ptr<ast::FunctionExpr> Parser::process_function_expr() {
        breakpoint(); logger.debug("process_function_expr()");
        if(!is_end() && !match(TokenType::IDENTIFIER)) throw expected_function_name();

        auto name_loc = peek().pos;
        auto node_loc = name_loc;
        std::string name = advance().sym;

        // useless - already checked by process_name() to call this
        if(!is_end() && !match(TokenType::LPAREN)) throw expected_lparen();
        skip(); // skip '('

        // todo: file end check
        std::vector<ast::ExprPtr> args;
        while(!is_end() && !match(TokenType::RPAREN)) {
            args.emplace_back(process_expr());
            if(!is_end() && match(TokenType::RPAREN)) break;
            if(!is_end() && !match(TokenType::COMMA)) throw expected_comma();
            skip(); // skip comma
        } if(is_end()) throw expected_rparen();
        node_loc.merge(advance().pos); // skip ')'
        return std::make_unique<ast::FunctionExpr>(
            std::string_view(name),
            std::move(args),
            std::move(name_loc),
            std::move(node_loc)
        );
    }

    std::unique_ptr<ast::VariableExpr> Parser::process_variable_expr() {
        breakpoint(); logger.debug("process_variable_expr()");
        if(!is_end(1) && !match(TokenType::IDENTIFIER)) throw expected_variable_name();
        auto name_loc = peek().pos;
        std::string name = advance().sym;
        return std::make_unique<ast::VariableExpr>(
            name,
            std::move(name_loc)
        );
    }

    // literals

    std::unique_ptr<ast::LiteralExpr> Parser::process_literal() {
        breakpoint(); logger.debug("process_literal()");
        switch (peek().ty) {
            case(TokenType::NUMBER): return process_number_literal();
            case(TokenType::STRING): return process_string_literal();
            case(TokenType::TRUE):   return process_bool_literal();
            case(TokenType::FALSE):  return process_bool_literal();
            default: {
                throw unexpected_token();
            }
        }
    }

    std::unique_ptr<ast::NumberLiteral> Parser::process_number_literal() {
        breakpoint(); logger.debug("process_number_literal()");
        // useless - already checked by process_literal() to call this
        // if(!is_end() && !match(TokenType::NUMBER)) throw expected_number();
        return std::make_unique<ast::NumberLiteral>(
            peek().sym,
            advance().pos
        );
    }

    std::unique_ptr<ast::StringLiteral> Parser::process_string_literal() {
        breakpoint(); logger.debug("process_string_literal()");
        // useless - already checked by process_literal() to call this
        // if(!is_end() && !match(TokenType::STRING)) throw expected_string();
        return std::make_unique<ast::StringLiteral>(
            peek().sym,
            advance().pos
        );
    }

    std::unique_ptr<ast::BoolLiteral> Parser::process_bool_literal() {
        breakpoint(); logger.debug("process_bool_literal()");
        // useless - already checked by process_literal() to call this
        // if(!is_end() && !match(TokenType::TRUE) || !match(TokenType::FALSE)) throw expected_bool();
        return std::make_unique<ast::BoolLiteral>(
            peek().sym,
            advance().pos
        );
    }

// diagnostic creating
    
    diagnostic::ParserError Parser::tokens_nullptr(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError("tokens = nullptr", common::SourceLocation());
        else return diagnostic::ParserError("tokens = nullptr", common::SourceLocation());
    }
    diagnostic::ParserError Parser::is_end_with_zero(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError("is_end() called with 0", common::SourceLocation());
        else return diagnostic::ParserError("is_end() called with 0", common::SourceLocation());
    }
    diagnostic::ParserError Parser::peek_out_of_range(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError("peek(): out of range", common::SourceLocation());
        else return diagnostic::ParserError("peek(): out of range", common::SourceLocation());
    }
    diagnostic::ParserError Parser::putback_out_of_range(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError("putback(): out of range", common::SourceLocation());
        else return diagnostic::ParserError("putback(): out of range", common::SourceLocation());
    }
    diagnostic::ParserError Parser::end_reached(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError("end reached", common::SourceLocation());
        else return diagnostic::ParserError("end reached", common::SourceLocation());
    }
    diagnostic::ParserError Parser::strcut_is_not_suported(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError("strcut is currently not suported", common::SourceLocation());
        else return diagnostic::ParserError("strcut is currently not suported", common::SourceLocation());
    }
    diagnostic::ParserError Parser::enum_is_not_suported(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError("enum is currently not suported", common::SourceLocation());
        else return diagnostic::ParserError("enum is currently not suported", common::SourceLocation());
    }
    diagnostic::ParserError Parser::if_is_not_suported(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError("if is currently not suported", common::SourceLocation());
        else return diagnostic::ParserError("if is currently not suported", common::SourceLocation());
    }
    diagnostic::ParserError Parser::else_is_not_suported(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError("else is currently not suported", common::SourceLocation());
        else return diagnostic::ParserError("else is currently not suported", common::SourceLocation());
    }
    diagnostic::ParserError Parser::for_is_not_suported(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError("for is currently not suported", common::SourceLocation());
        else return diagnostic::ParserError("for is currently not suported", common::SourceLocation());
    }
    diagnostic::ParserError Parser::while_is_not_suported(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError("while is currently not suported", common::SourceLocation());
        else return diagnostic::ParserError("while is currently not suported", common::SourceLocation());
    }
    diagnostic::ParserError Parser::break_is_not_suported(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError("break is currently not suported", common::SourceLocation());
        else return diagnostic::ParserError("break is currently not suported", common::SourceLocation());
    }
    diagnostic::ParserError Parser::continue_is_not_suported(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError("continue is currently not suported", common::SourceLocation());
        else return diagnostic::ParserError("continue is currently not suported", common::SourceLocation());
    }
    diagnostic::ParserError Parser::stack_initialization_not_supported(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError("stack initialization is currently not suported", common::SourceLocation());
        else return diagnostic::ParserError("stack initialization is currently not suported", common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_semicolon(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected ';', got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected ';', got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_lbracket(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected '[', got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected '[', got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_rbracket(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected ']', got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected ']', got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_lbrace(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected '{{', got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected '{{', got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_rbrace(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected '}}', got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected '}}', got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_lparen(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected '(', got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected '(', got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_rparen(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected ')', got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected ')', got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_comma(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected ',', got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected ',', got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_doublecolon(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected '::', got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected '::', got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_unary_op(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected unary operator, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected unary operator, got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_postfix_op(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected postfix operator, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected postfix operator, got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_identifier(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected identifier, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected identifier, got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_function_name(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected function name, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected function name, got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_variable_name(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected variable name, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected variable name, got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_module_name(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected module name, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected module name, got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_submodule_name(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected submodule name, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected submodule name, got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_type(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected type, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected type, got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_number(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected number, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected number, got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_string(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected string, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected string, got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::expected_bool(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("expected bool, got {}", utils::stringify(peek(offset).ty)), peek(offset).pos);
        else return diagnostic::ParserError(std::format("expected bool, got buf file had ended"), common::SourceLocation());
    }
    diagnostic::ParserError Parser::unexpected_token(size_t offset) const noexcept {
        if(!is_end()) return diagnostic::ParserError(std::format("unexpected token: {}"
        ,                       utils::stringify(peek(offset).ty))
        ,                   peek(offset).pos
        ); else return diagnostic::ParserError(std::format("unexpected token in end of file (strange error, possibly have some inter error in logic)"));;
    }
}
