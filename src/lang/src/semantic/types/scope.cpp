#include "lang/ast/ast.h"
#include "lang/common/symbol_path.h"
#include <climits>
#include <lang/utils/diagnostic.h>
#include <common/diagnostic/diagnostic.h>
#include <lang/semantic/types/identifier.h>

#include <lang/semantic/types/scope.h>
#include <memory>

namespace lang::semantic
{
// Scope

    bool Scope::is_global() const noexcept {
        return parent == nullptr;
    }

    const Scope* Scope::get_parent() const noexcept {
        return  parent;
    }
    Scope* Scope::get_parent() noexcept {
        return  parent;
    }

    bool Scope::contains(SymbolID id) const noexcept {
        if(contains_local(id)) return true;
        if(!is_global()) return parent->contains(id);
        return false;
    }

    bool Scope::contains_local(SymbolID id) const noexcept {
        return symbols.contains(id);
    }

    void Scope::add_symbol(SymbolID id, std::unique_ptr<Symbol> symbol) {
        // if symbol is new
        if(!contains(id)) {
            symbols[id] = std::move(symbol);
            return;
        }
        // if already contains - updating info
        if(symbol->decl != nullptr) symbols.at(id)->decl = symbol->decl;
        if(symbol->kind != Symbol::Kind::NOT_VALID) symbols.at(id)->kind = symbol->kind;
        if(!symbol->sympath.empty()) symbols.at(id)->sympath = symbol->sympath;
    }

    const Symbol* Scope::get(SymbolID id) const noexcept {
        if(!contains(id)) return nullptr;
        if(contains_local(id)) return symbols.at(id).get();
        return parent->get(id);
    }

    Symbol& Scope::at(SymbolID id) {
        if(!contains(id)) throw common::diagnostic::InterError("Scope: bad at()");
        if(contains_local(id)) return *symbols.at(id);
        return parent->at(id);
    }

// SymbolsContext

    SymbolsContext::SymbolsContext() {
        global_scope = std::make_unique<Scope>();
        current_scope = global_scope.get();
    }

    void SymbolsContext::try_complete(Symbol* symbol) const {
        if(symbol->id.id != ULLONG_MAX) {

            if(!symbol->sympath.empty()
            && sympaths_contexts.contains(symbol->sympath)) symbol->id = sympaths_contexts.at(symbol->sympath);

            // try to get by ast node
            if(symbol->decl != nullptr
            && nodes_contexts.contains((ast::BaseNode*)symbol->decl)) symbol->id = nodes_contexts.at((ast::BaseNode*)symbol->decl);
        }
    }

    bool SymbolsContext::contains(Symbol symbol) const noexcept {
        if(symbol.id.id == ULLONG_MAX) try_complete(&symbol);
        if(symbol.id.id == ULLONG_MAX) return false;
        return true;
    }

    bool SymbolsContext::contains(SymbolID id) const noexcept {
        return symbols.contains(id);
    }

    void SymbolsContext::enter_scope(SymbolID id) {
        if(!contains(id)) throw common::diagnostic::InterError("enter_scope(): given id is not contains");
        if(scope_context.contains(id)) current_scope = scope_context.at(id).get();
        else {
            // creating new scope with current - current scope
            auto scope = std::make_unique<Scope>(current_scope);
            current_scope = scope.get();
            scope_context[id] = std::move(scope);
        }
    }

    void SymbolsContext::leave_scope() noexcept {
        if(current_scope->is_global()) current_scope = global_scope.get();
        else current_scope = current_scope->get_parent();
    }

    void SymbolsContext::enter_global_scope() noexcept {
        current_scope = global_scope.get();
    }

    SymbolID SymbolsContext::add(std::unique_ptr<Symbol> symbol) {
        SymbolID id;

        // creating new id or just use given
        if(!contains(*symbol)) {
            id.id = counter++;
            symbol->id = id;
        } else id = symbol->id;

        symbols[id] = symbol.get();
        if(symbol->decl != nullptr) nodes_contexts[(ast::BaseNode*)symbol->decl] = id;
        if(!symbol->sympath.empty()) sympaths_contexts[symbol->sympath] = id;

        current_scope->add_symbol(id, std::move(symbol));
        return id;
    }

    // void SymbolContext::update_contexts(Symbol symbol) {
    //     if(!contains(symbol)) return;
    //     try_complete(&symbol);
    //     if(symbol.decl != nullptr) symbols.at(symbol.id)->decl = symbol.decl;
    //     if(!symbol.sympath.empty()) symbols.at(symbol.id)->sympath = symbol.sympath;
    // }

    const Symbol* SymbolsContext::get(SymbolID id) const noexcept {
        if(!contains(id)) return nullptr;
        return symbols.at(id);
    }

    Symbol* SymbolsContext::get(SymbolID id) noexcept {
        if(!contains(id)) return nullptr;
        return symbols.at(id);
    }

    const Symbol* SymbolsContext::get(const ast::BaseNode* node) const noexcept {
        if(!nodes_contexts.contains(node)) return nullptr;
        return symbols.at(nodes_contexts.at(node));
    }

    Symbol* SymbolsContext::get(const ast::BaseNode* node) noexcept {
        if(!nodes_contexts.contains(node)) return nullptr;
        return symbols.at(nodes_contexts.at(node));
    }

    const Symbol* SymbolsContext::get(SymbolPath sympath) const noexcept {
        if(!sympaths_contexts.contains(sympath)) return nullptr;
        return symbols.at(sympaths_contexts.at(sympath));
    }

}
