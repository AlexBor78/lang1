#include "ast/expr.h"
#include <print>
#include <utility>
#include <utils/utils.h>
#include <utils/printast.h>

namespace lang::utils
{
    void PrintAST::remove_from_prefix(int count) noexcept
    {
        if(count > prefix.length())
            return;
        
        for(int i =0; i < count; ++i)
            prefix.pop_back();
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

    void PrintAST::print(const ast::BaseNode* node) noexcept
    {
        if(!node) return;
        clean_prefix();
        node->accept(*this);
    }

    void PrintAST::visit_block_stmt(const ast::BlockStmt& node) noexcept
    {
        println("BlockStmt:");
        
        add_to_prefix();
        for(const auto& stmt : node.get_body())
            if(stmt) stmt->accept(*this);
        remove_from_prefix();
    }

    void PrintAST::visit_if_stmt(const ast::IfStmt& node) noexcept
    {
        println("IfStmt:");
        add_to_prefix();
        
        print("Condition:");
        add_to_prefix();
        if(node.get_cond()) node.get_cond()->accept(*this);
        else println("null");
        remove_from_prefix();

        print("Body:");
        add_to_prefix();
        if(node.get_body()) node.get_body()->accept(*this);
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_for_stmt(const ast::ForStmt& node) noexcept
    {
        println("ForStmt:");
        add_to_prefix();
        
        print("Declaration:");
        add_to_prefix();
        if(node.get_decl()) node.get_decl()->accept(*this);
        else println("null");
        remove_from_prefix();

        print("Condition:");
        add_to_prefix();
        if(node.get_cond()) node.get_cond()->accept(*this);
        else println("null");
        remove_from_prefix();

        print("Increment:");
        add_to_prefix();
        if(node.get_incr()) node.get_incr()->accept(*this);
        else println("null");
        remove_from_prefix();

        print("Body:");
        add_to_prefix();
        if(node.get_body()) node.get_body()->accept(*this);
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_while_stmt(const ast::WhileStmt& node) noexcept
    {
        println("WhileStmt:");
        add_to_prefix();
        
        print("Condition:");
        add_to_prefix();
        if(node.get_cond()) node.get_cond()->accept(*this);
        else println("null");
        remove_from_prefix();

        print("Body:");
        add_to_prefix();
        if(node.get_body()) node.get_body()->accept(*this);
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_decl_var(const ast::DeclVar& node) noexcept
    {
        println("DeclVar:");
        add_to_prefix();

        println("name: {}", node.get_name());
        
        print("init: ");
        add_to_prefix();
        if(node.get_init_expr()) node.get_init_expr()->accept((*this));
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_decl_func(const ast::DeclFunc& node) noexcept
    {
        println("DeclFunc:");
        add_to_prefix();
        println("name: {}", node.get_name());
        
        print("args: ");
        add_to_prefix();
        if(!node.get_args().empty()) {
            for(const auto& arg : node.get_args())
                if(arg) arg->accept(*this);
        }
        else println("null");
        remove_from_prefix();
        
        print("body: ");
        add_to_prefix();
        if(node.get_body()) node.get_body()->accept(*this);
        else println("null");

        remove_from_prefix(4);
    }

    void PrintAST::visit_decl_module(const ast::DeclModule& node) noexcept
    {
        println("DeclModule:");
        add_to_prefix();
        println("name: {}", node.get_name());
        remove_from_prefix();
    }
    void PrintAST::visit_decl_namespace(const ast::DeclNamespace& node) noexcept
    {
        println("DeclNamespace:");
        add_to_prefix();
        println("name: {}", node.get_name());
        print("body: ");
        add_to_prefix();
        if(node.get_body()) node.get_body()->accept(*this);
        else println("null");

        remove_from_prefix();
    }
    void PrintAST::visit_import_stmt(const ast::ImportStmt& node) noexcept
    {
        println("ImportStmt:");
        add_to_prefix();
        println("name: {}", node.get_imported());
        remove_from_prefix();
    }
    void PrintAST::visit_return_stmt(const ast::ReturnStmt& node) noexcept
    {
        println("ReturnStmt:");
        add_to_prefix();
        print("return expr: ");
        add_to_prefix();
        if(node.get_ret_expr()) node.get_ret_expr()->accept(*this);
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_number_literal(const ast::NumberLiteral& node) noexcept
    {
        println("NumberLiteral:");
        add_to_prefix();
        println("literal: {}", node.get_literal());
        remove_from_prefix();
    }
    void PrintAST::visit_string_literal(const ast::StringLiteral& node) noexcept
    {
        println("StringLiteral:");
        add_to_prefix();
        println("literal: \"{}\"", node.get_literal());
        remove_from_prefix();
    }
    void PrintAST::visit_bool_literal(const ast::BoolLiteral& node) noexcept
    {
        println("BoolLiteral:");
        add_to_prefix();
        println("literal: {}", node.get_literal());
        remove_from_prefix();
    }
    void PrintAST::visit_variable_expr(const ast::VariableExpr& node) noexcept
    {
        println("VariableExpr:");
        add_to_prefix();
        println("name: {}", node.get_name());
        remove_from_prefix();
    }
    void PrintAST::visit_namespace_expr(const ast::NamespaceExpr& node) noexcept
    {
        println("NamespaceExpr:");
        add_to_prefix();
        println("name: {}", node.get_name());
        
        print("identifier: ");
        add_to_prefix();
        if(node.get_identifier()) node.get_identifier()->accept((*this));
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_call_expr(const ast::CallExpr& node) noexcept
    {
        println("CallExpr:");
        add_to_prefix();
        println("name: {}", node.get_callee());

        print("args: ");
        add_to_prefix();
        if(!node.get_args().empty()) {
            for(const auto& arg : node.get_args())
                if(arg) arg->accept(*this);
        } else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_binop_expr(const ast::BinOpExpr& node) noexcept
    {
        println("BinOpExpr:");
        add_to_prefix();
        println("operator: {}", stringify(node.get_op()));
        print("left expr: ");
        add_to_prefix();
        if(node.get_left()) node.get_left()->accept(*this);
        else println("null");
        remove_from_prefix();

        print("right expr: ");
        add_to_prefix();
        if(node.get_right()) node.get_right()->accept(*this);
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_prefix_unary_op_expr(const ast::PrefixUnaryOpExpr& node) noexcept
    {
        println("PrefixUnaryOpExpr:");
        add_to_prefix();
        println("operator: {}", stringify(node.get_op()));

        print("operand expr: ");
        add_to_prefix();
        if(node.get_operand()) node.get_operand()->accept(*this);
        else println("null");
        remove_from_prefix(4);
    }
    void PrintAST::visit_postfix_unary_op_expr(const ast::PostfixUnaryOpExpr& node) noexcept
    {
        println("PostfixUnaryOpExpr:");
        add_to_prefix();
        println("operator: {}", stringify(node.get_op()));

        print("operand expr: ");
        add_to_prefix();
        if(node.get_operand()) node.get_operand()->accept(*this);
        else println("null");
        remove_from_prefix(4);
    }
}
