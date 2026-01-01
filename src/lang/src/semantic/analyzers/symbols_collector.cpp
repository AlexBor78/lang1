#include <lang/semantic/types/identifier.h>
#include <lang/semantic/types/semantic_types.h>
#include <lang/semantic/analyzers/symbols_collector.h>

namespace lang::semantic
{
    void SymbolsCollector::collect(SemanticState* _state) {
        state = _state;
        collect();
    }

    void SymbolsCollector::collect() {
        for(const auto& module_ : state->program.modules) {
            for(const auto& node : module_.second->ast) {
                node->accept(*this);
            }
        }
    }

    void SymbolsCollector::visit_block_stmt(ast::BlockStmt& node) noexcept {
        for(const auto& stmt : node.get_body()) {
            if(stmt) stmt->accept(*this);
        }
    }
    void SymbolsCollector::visit_decl_var(ast::DeclVariable& node) noexcept {
        if(state->curr_scope->contains(std::string(node.get_name()))) return;
        state->curr_scope->add_identifier(Identifier::create(node.get_name(), Identifier::Kind::VARIABLE, &node));
    }
    void SymbolsCollector::visit_decl_func(ast::DeclFunction& node) noexcept {
        if(state->curr_scope->contains(std::string(node.get_name()))) return;
        state->curr_scope->add_identifier(Identifier::create(node.get_name(), Identifier::Kind::FUNCTOIN, &node));
    }
}
