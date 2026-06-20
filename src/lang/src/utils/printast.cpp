#include <print>
#include <utility>
#include <lang/utils/printast.h>
#include <lang/utils/syntax_utils.h>

namespace lang::utils
{
    void PrintAST::remove_from_prefix(size_t count) noexcept
    {
        if(count > prefix.length())
            return;
        
        for(size_t i = 0; i < count; ++i) prefix.pop_back();
    }

    void PrintAST::add_to_prefix(std::string_view str) noexcept
    {
        prefix += str;
    }

    void PrintAST::clean_prefix() noexcept
    {
        prefix = "";
    }

    template<typename... Args>
    void PrintAST::print(std::format_string<Args...> fmt, Args&&... args) noexcept
    {
        if(need_prefix) std::print("{}{}", prefix, std::format(fmt, std::forward<Args>(args)...));
        else std::print("{}", std::format(fmt, std::forward<Args>(args)...));
        need_prefix = false;
    }

    template<typename... Args>
    void PrintAST::println(std::format_string<Args...> fmt, Args&&... args) noexcept
    {
        if(need_prefix) std::print("{}{}\n", prefix, std::format(fmt, std::forward<Args>(args)...));
        else std::print("{}\n", std::format(fmt, std::forward<Args>(args)...));
        need_prefix = true;
    }

    void PrintAST::print(const syntax::ast::BaseNode* node) noexcept
    {
        if(!node) return;
        clean_prefix();
        node->accept(*this);
    }

    void PrintAST::visit_block_stmt(const syntax::ast::BlockStmt& node) noexcept
    {
        println("BlockStmt:");
        
        add_to_prefix();
        for(const auto& stmt : node.body)
            if(stmt) stmt->accept(*this);
        remove_from_prefix();
    }

    void PrintAST::visit_if_stmt(const syntax::ast::IfStmt& node) noexcept
    {
        println("IfStmt:");
        add_to_prefix();
        
        print("Condition:");
        add_to_prefix();
        if(node.cond) node.cond->accept(*this);
        else println("null");
        remove_from_prefix();

        print("Body:");
        add_to_prefix();
        if(node.body) node.body->accept(*this);
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_else_stmt(const syntax::ast::ElseStmt& node) noexcept
    {
        println("ElseStmt:");
        add_to_prefix();
        
        print("Body:");
        add_to_prefix();
        if(node.body) node.body->accept(*this);
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_for_stmt(const syntax::ast::ForStmt& node) noexcept
    {
        println("ForStmt:");
        add_to_prefix();
        
        print("Declaration:");
        add_to_prefix();
        if(node.decl) node.decl->accept(*this);
        else println("null");
        remove_from_prefix();

        print("Condition:");
        add_to_prefix();
        if(node.cond) node.cond->accept(*this);
        else println("null");
        remove_from_prefix();

        print("Increment:");
        add_to_prefix();
        if(node.incr) node.incr->accept(*this);
        else println("null");
        remove_from_prefix();

        print("Body:");
        add_to_prefix();
        if(node.body) node.body->accept(*this);
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_while_stmt(const syntax::ast::WhileStmt& node) noexcept
    {
        println("WhileStmt:");
        add_to_prefix();
        
        print("Condition:");
        add_to_prefix();
        if(node.cond) node.cond->accept(*this);
        else println("null");
        remove_from_prefix();

        print("Body:");
        add_to_prefix();
        if(node.body) node.body->accept(*this);
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_decl_var(const syntax::ast::DeclVariable& node) noexcept
    {
        println("DeclVar:");
        add_to_prefix();

        println("name: {}", node.name);
        
        print("init: ");
        add_to_prefix();
        if(node.init_expr) node.init_expr->accept((*this));
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_decl_func(const syntax::ast::DeclFunction& node) noexcept
    {
        println("DeclFunc:");
        add_to_prefix();
        println("name: {}", node.name);
        
        print("args: ");
        add_to_prefix();
        if(!node.args.empty()) {
            for(const auto& arg : node.args)
                if(arg) arg->accept(*this);
        }
        else println("null");
        remove_from_prefix();
        
        print("body: ");
        add_to_prefix();
        if(node.body) node.body->accept(*this);
        else println("null");

        remove_from_prefix(4);
    }

    void PrintAST::visit_import_stmt(const syntax::ast::ImportStmt& node) noexcept
    {
        println("ImportStmt:");
        add_to_prefix();
        println("name: {}", node.path.to_string());
        remove_from_prefix();
    }
    void PrintAST::visit_return_stmt(const syntax::ast::ReturnStmt& node) noexcept
    {
        println("ReturnStmt:");
        add_to_prefix();
        print("return expr: ");
        add_to_prefix();
        if(node.ret_expr) node.ret_expr->accept(*this);
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_break_stmt(const syntax::ast::BreakStmt&) noexcept {
        println("BreakStmt:");
    }
    void PrintAST::visit_continue_stmt(const syntax::ast::ContinueStmt&) noexcept {
        println("ContinueStmt:");
    }
    void PrintAST::visit_number_literal(const syntax::ast::NumberLiteral& node) noexcept
    {
        println("NumberLiteral:");
        add_to_prefix();
        println("literal: {}", node.literal);
        remove_from_prefix();
    }
    void PrintAST::visit_string_literal(const syntax::ast::StringLiteral& node) noexcept
    {
        println("StringLiteral:");
        add_to_prefix();
        println("literal: \"{}\"", node.literal);
        remove_from_prefix();
    }
    void PrintAST::visit_bool_literal(const syntax::ast::BoolLiteral& node) noexcept
    {
        println("BoolLiteral:");
        add_to_prefix();
        println("literal: {}", node.literal);
        remove_from_prefix();
    }
    void PrintAST::visit_variable_expr(const syntax::ast::VariableExpr& node) noexcept
    {
        println("VariableExpr:");
        add_to_prefix();
        println("name: {}", node.name);
        remove_from_prefix();
    }
    void PrintAST::visit_stackalloc_expr(const syntax::ast::StackAllocExpr& node) noexcept {
        println("StackAllocExpr:");
        add_to_prefix();
        print("dimensions: ");
        for(const auto size : node.dimensions) {
            print("[{}]", size);
        }
        remove_from_prefix();
    }
    void PrintAST::visit_symbol_path(const syntax::ast::SymbolPathExpr& node) noexcept
    {
        println("NamespaceExpr:");
        add_to_prefix();
        println("name: {}", node.name);
        
        print("identifier: ");
        add_to_prefix();
        if(node.identifier) node.identifier->accept((*this));
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_call_expr(const syntax::ast::FunctionExpr& node) noexcept
    {
        println("CallExpr:");
        add_to_prefix();
        println("name: {}", node.callee);

        print("args: ");
        add_to_prefix();
        if(!node.args.empty()) {
            for(const auto& arg : node.args)
                if(arg) arg->accept(*this);
        } else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_binop_expr(const syntax::ast::BinOpExpr& node) noexcept
    {
        println("BinOpExpr:");
        add_to_prefix();
        println("operator: {}", stringify(node.op));
        print("left expr: ");
        add_to_prefix();
        if(node.left) node.left->accept(*this);
        else println("null");
        remove_from_prefix();

        print("right expr: ");
        add_to_prefix();
        if(node.right) node.right->accept(*this);
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_prefix_unary_op_expr(const syntax::ast::PrefixUnaryOpExpr& node) noexcept
    {
        println("PrefixUnaryOpExpr:");
        add_to_prefix();
        println("operator: {}", stringify(node.op));

        print("operand expr: ");
        add_to_prefix();
        if(node.operand) node.operand->accept(*this);
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_postfix_unary_op_expr(const syntax::ast::PostfixUnaryOpExpr& node) noexcept
    {
        println("PostfixUnaryOpExpr:");
        add_to_prefix();
        println("operator: {}", stringify(node.op));

        print("operand expr: ");
        add_to_prefix();
        if(node.operand) node.operand->accept(*this);
        else println("null");
        remove_from_prefix(4);
    }
}
