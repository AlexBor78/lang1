#pragma once

#include <memory>
#include <unordered_map>

#include <lang/ast/ast.h>
#include <lang/common/symbol_path.h>
#include <lang/semantic/types/identifier.h>
#include <lang/semantic/types/typesystem.h>

namespace lang::semantic 
{
    class Scope
    {
    private:
        /**
         * @brief pointer to parent scope; nullptr - it is global scope
         */
        Scope* parent{nullptr};

        /**
         * @brief list of symbol in scope
         */
        std::unordered_map<SymbolID, std::unique_ptr<Symbol>> symbols;

    public:
        explicit Scope(Scope* _parent = nullptr):
            parent(_parent)
        {}

        /**
         * @brief   check if this scope is global
         * @return  true 
         * @return  false 
         */
        bool is_global() const noexcept;

        /**
         * @brief   const getter for parent scope
         * @return  const Scope* 
         */
        const Scope* get_parent() const noexcept;

        /**
         * @brief   non-const getter for parent scope
         * @return  const Scope* 
         */
        Scope* get_parent() noexcept;

        /**
         * @brief   check if given symbol contains from this scope to global
         * @return  true 
         * @return  false 
         */
        bool contains(SymbolID) const noexcept;

        /**
         * @brief   check if given symbol contains in THIS scope only
         * @return  true 
         * @return  false 
         */
        bool contains_local(SymbolID) const noexcept;

        /**
         * @brief   store symbol in scope
         * @note    can be used to update symbol
         */
        void add_symbol(SymbolID, std::unique_ptr<Symbol>);

        /**
         * @brief   get symbol by ID
         * @return  Symbol 
         */
        const Symbol* get(SymbolID) const noexcept;

        /**
         * @brief   like get(), but return ref (you can change symbol)
         * @return  Symbol& 
         */
        Symbol& at(SymbolID);
    };

    /**
     * @brief   main storage for all symbols
     * @note    control SymbolIDs
     */
    class SymbolsContext
    {
    private:

        /**
         * @brief   counter for creating unique id
         */
        size_t counter{0};

        /**
         * @brief   pointer to global scope
         * @note    global scope is stored as it
         */
        std::unique_ptr<Scope> global_scope{nullptr};

        /**
         * @brief   pointer to current scope
         */
        Scope* current_scope{nullptr};

        /**
         * @brief   save almost ALL scopes. 
         * @note    interfaces of modules (public scopes) also stored here
         * @note    algorithm to find scope by SymbolPath:
                    1. go to global scope
                    2. find Identifier
                    3. in Identifier usably there is pointer to Node
                        1. if there is: go to context and get scope
                        2. if not - it is module, find module by ModuleID in program, in module there is it's scope
         */
        std::unordered_map<SymbolID, std::unique_ptr<Scope>> scope_context;

        /**
         * @brief   store pointers to all symbols for fast access
         */
        std::unordered_map<SymbolID, Symbol*> symbols;

        /**
         * @brief   context to store module's internal scopes
         */
        std::unordered_map<SymbolID, std::unique_ptr<Scope>> internal_scopes;

        /**
         * @brief   save's types of declarations, bcs ast dont
         * @note    full types, with wrappers, but BASE type is owned by types_table
         */
        std::unordered_map<SymbolID, std::unique_ptr<AbstractType>> types_context;
        
        /**
         * @brief   table of types, save BASE types only, not conflict with types_context.
         */
        TypeTable types_table;

        /**
         * @brief   context ASTNode -> SymbolID
         */
        std::unordered_map<const ast::BaseNode*, SymbolID> nodes_contexts;
        
        /**
         * @brief   context SymbolPath -> SymbolID
         * 
         */
        std::unordered_map<SymbolPath, SymbolID> sympaths_contexts;

        /**
         * @brief   trying to complete symbol info by contexts
         * @note    don't change already containing info
         * @return  Symbol 
         */
        void try_complete(Symbol*) const;

    public:

        /**
         * @brief   Construct a new Symbol Context object
         * @note    creating global scope
         */
        SymbolsContext();
        ~SymbolsContext() = default;

        /**
         * @brief   check if symbol contains
         * @return  true
         * @return  false
         */
        bool contains(Symbol) const noexcept;
        
        /**
         * @brief   check if symbol contains by id
         * @return  true 
         * @return  false 
         */
        bool contains(SymbolID) const noexcept;

        /**
         * @brief   entering scope of module
         * @note    crating if not existed
         * @note    befor entering scope, (e.g. module, functoin, etc) you need to create symbol
         * @note    creating new scope, with parent - current scope
         * @exception InterError when id is not contains
         */
        void enter_scope(SymbolID);

        /**
         * @brief   escaping scope, go to it's parent
         */
        void leave_scope() noexcept;

        /**
         * @brief   just go to global scope
         */
        void enter_global_scope() noexcept;
        
        /**
         * @brief   Add new symbol or update old info (but not contexts)
         */
        SymbolID add(std::unique_ptr<Symbol>);

        /**
         * @brief   const symbol getter by id
         * @note    return nullptr if not contains
         * @return  const Symbol* return nullptr if not contains
         */
        const Symbol* get(SymbolID) const noexcept;
        
        /**
         * @brief   non-const symbol getter by id
         * @note    return nullptr if not contains
         * @return  const Symbol* return nullptr if not contains
         */
        Symbol* get(SymbolID) noexcept;

        /**
         * @brief   const symbol getter by nodeptr
         * @note    return nullptr if not contains
         * @return  const Symbol* return nullptr if not contains
         */
        const Symbol* get(const ast::BaseNode*) const noexcept;

        /**
         * @brief   non-const symbol getter by nodeptr
         * @note    return nullptr if not contains
         * @return  const Symbol* return nullptr if not contains
         */
        Symbol* get(const ast::BaseNode*) noexcept;

        /**
         * @brief   const symbol getter by sympath
         * @note    return nullptr if not contains
         * @return  const Symbol* return nullptr if not contains
         */
        const Symbol* get(SymbolPath) const noexcept;

        /**
         * @brief   non-const symbol getter by sympath
         * @note    return nullptr if not contains
         * @return  const Symbol* return nullptr if not contains
         */
        Symbol* get(SymbolPath) noexcept;
    };
}
