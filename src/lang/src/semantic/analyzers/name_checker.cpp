#pragma once

#include <memory>
#include <format>
#include <common/diagnostic/diagnostic.h>
#include <lang/semantic/types/scope.h>
#include <lang/semantic/analyzers/name_checker.h>

namespace lang::semantic
{
    void NameChecker::analyze(SemanticState* _state) {
        state = _state;
        analyze();
    }

    void NameChecker::analyze() {
        for(const auto& module_ : state->program.modules) {
            for(const auto& node : module_.second->ast) {
                node->accept(*this);
            }
        }
    }

    void NameChecker::visit_block_stmt(const ast::BlockStmt& block) noexcept {
        for(const auto& node : block.get_body()) {
            if(node) node->accept(*this);
        }
    }
    void NameChecker::visit_if_stmt(const ast::IfStmt& node) noexcept {
        state->enter_scope(state->context.add_scope(&node, state->curr_scope));
        if(node.get_body()) node.get_body()->accept(*this);
        state->leave_scope();
    }
    void NameChecker::visit_else_stmt(const ast::ElseStmt& node) noexcept {
        state->enter_scope(state->context.add_scope(&node, state->curr_scope));
        if(node.get_body()) node.get_body()->accept(*this);
        state->leave_scope();
    }
    void NameChecker::visit_for_stmt(const ast::ForStmt& node) noexcept {
        state->enter_scope(state->context.add_scope(&node, state->curr_scope));
        if(node.get_decl()) node.get_decl()->accept(*this);
        if(node.get_cond()) node.get_cond()->accept(*this);
        if(node.get_incr()) node.get_incr()->accept(*this);
        if(node.get_body()) node.get_body()->accept(*this);
        state->leave_scope();
    }
    void NameChecker::visit_while_stmt(const ast::WhileStmt& node) noexcept {
        state->enter_scope(state->curr_scope->add_child());
        if(node.get_cond()) node.get_cond()->accept(*this);
        if(node.get_body()) node.get_body()->accept(*this);
        state->leave_scope();
    }
    void NameChecker::visit_decl_var(const ast::DeclVariable& node) noexcept {
        if(state->curr_scope->contains_local(std::string(node.get_name()))) 
        throw common::diagnostic::InterError(std::format("Variable {} redefinition:", node.get_name()));
        // todo: make and throw better error with pos
        // state->curr_scope->add_identifier(Identifier::create(node.get_name(), Identifier::Kind::VARIABLE, &node));

    }
    void NameChecker::visit_decl_func(const ast::DeclFunction& node) noexcept {

    }
    void NameChecker::visit_decl_module(const ast::DeclModule& node) noexcept {

    }
    void NameChecker::visit_decl_namespace(const ast::DeclNamespace& node) noexcept {

    }
    void NameChecker::visit_return_stmt(const ast::ReturnStmt& node) noexcept {

    }
    void NameChecker::visit_variable_expr(const ast::VariableExpr& node) noexcept {

    }
    void NameChecker::visit_namespace_expr(const ast::NamespaceExpr& node) noexcept {

    }
    void NameChecker::visit_call_expr(const ast::FunctionExpr& node) noexcept {

    }
    void NameChecker::visit_binop_expr(const ast::BinOpExpr& node) noexcept {

    }
    void NameChecker::visit_prefix_unary_op_expr(const ast::PrefixUnaryOpExpr& node) noexcept {

    }
    void NameChecker::visit_postfix_unary_op_expr(const ast::PostfixUnaryOpExpr& node) noexcept {

    }
}
