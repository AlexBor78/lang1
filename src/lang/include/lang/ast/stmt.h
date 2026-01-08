#pragma once

#include <memory>
#include <vector>
#include <string_view>
#include <lang/ast/ast.h>
#include <lang/common/symbol_path.h>

namespace lang::ast
{   
    class BlockStmt : public StmtNode
    {
    private:
        std::vector<StmtPtr> m_body;
        
    public:
        explicit BlockStmt(common::SourceLocation _pos = default_pos()):
            StmtNode(std::move(_pos))
        {}

        explicit BlockStmt(
            std::vector<StmtPtr>& _body
        ,   common::SourceLocation _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   m_body(std::move(_body))
        {}

        explicit BlockStmt(
            std::vector<StmtPtr> _body
        ,   common::SourceLocation _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   m_body(std::move(_body))
        {}
        
        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        const std::vector<StmtPtr>& get_body() const noexcept;
        std::vector<StmtPtr>& get_body() noexcept;

        void add_tobody(StmtPtr) noexcept;
    };

    class StructureStmt : public StmtNode
    {
    private:
        StmtPtr body;
        common::SourceLocation keyword_loc;

    protected:
        explicit StructureStmt(StmtPtr _body
        ,                      common::SourceLocation _word_loc = default_pos()
        ,                      common::SourceLocation _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   body(std::move(_body))
        ,   keyword_loc(_word_loc)
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;

        const StmtNode* get_body() const;
        StmtNode* get_body();

        const common::SourceLocation& get_keyword_loc() const noexcept;
    };

    class StructureStmtWithCond : public StructureStmt
    {
    private:
        ExprPtr cond;

    protected:
        StructureStmtWithCond(ExprPtr _cond
        ,                     StmtPtr _body
        ,                     common::SourceLocation _word_loc = default_pos()
        ,                     common::SourceLocation _pos = default_pos()
        ):  StructureStmt(std::move(_body)
            ,             std::move(_word_loc)
            ,             std::move(_pos)
            )
        ,   cond(std::move(_cond))
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;

        const ExprNode* get_cond() const;
        ExprNode* get_cond();
    };

    class IfStmt : public StructureStmtWithCond
    {
    public:
        IfStmt(ExprPtr _cond
        ,      StmtPtr _body
        ,      common::SourceLocation _word_loc = default_pos()
        ,      common::SourceLocation _pos = default_pos()
        ):  StructureStmtWithCond(std::move(_cond)
            ,                     std::move(_body)
            ,                     std::move(_word_loc)
            ,                     std::move(_pos)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class ElseStmt : public StructureStmt
    {
    public:
        explicit ElseStmt(StmtPtr _body
        ,                 common::SourceLocation _word_loc = default_pos()
        ,                 common::SourceLocation _pos = default_pos()
        ):  StructureStmt(std::move(_body)
            ,             std::move(_word_loc)
            ,             std::move(_pos)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class ForStmt : public StructureStmtWithCond
    {
    private:
        StmtPtr decl;
        StmtPtr incr;

    public:
        ForStmt(StmtPtr _decl
        ,       ExprPtr _cond
        ,       StmtPtr _incr
        ,       StmtPtr _body
        ,       common::SourceLocation _word_loc = default_pos()
        ,       common::SourceLocation _pos = default_pos()
        ):  StructureStmtWithCond(std::move(_cond)
            ,             std::move(_body)
            ,             std::move(_word_loc)
            ,             std::move(_pos)
            )
        ,   decl(std::move(_decl))
        ,   incr(std::move(_incr))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        const StmtNode* get_decl() const;
        StmtNode* get_decl();

        const StmtNode* get_incr() const;
        StmtNode* get_incr();
    };

    class WhileStmt : public StructureStmtWithCond
    {
    public:
        WhileStmt(ExprPtr _cond
        ,         StmtPtr _body
        ,         common::SourceLocation _word_loc = default_pos()
        ,         common::SourceLocation _pos = default_pos()
        ):  StructureStmtWithCond(std::move(_cond)
            ,             std::move(_body)
            ,             std::move(_word_loc)
            ,             std::move(_pos)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class DeclStmt : public StmtNode
    {
    private:
        std::string name;
        common::SourceLocation name_loc;

    protected:
        DeclStmt(std::string_view _name
        ,        common::SourceLocation _name_loc = default_pos()
        ,        common::SourceLocation _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   name(_name)
        ,   name_loc(_name_loc)
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;

        std::string_view get_name() const noexcept;
        const common::SourceLocation& get_name_loc() const noexcept;
    };
    
    class DeclName : public DeclStmt
    {
    protected:
        DeclName(std::string_view _name
        ,        common::SourceLocation _name_loc = default_pos()
        ,        common::SourceLocation _pos = default_pos()
        ):  DeclStmt(_name
            ,        std::move(_name_loc)
            ,        std::move(_pos)
            )
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;

        bool is_extern() const noexcept;
    };

    class DeclVariable : public DeclName
    {
    private:
        ExprPtr init_expr;

    public:
        explicit DeclVariable(std::string_view _name
        ,                common::SourceLocation _name_loc = default_pos()
        ,                ExprPtr _init = nullptr
        ,                common::SourceLocation _pos = default_pos()
        ):  DeclName(_name
            ,        std::move(_name_loc)
            ,        std::move(_pos)
            )
        ,   init_expr(std::move(_init))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        const ExprNode* get_init_expr() const noexcept;
    };

    class DeclFunction : public DeclName
    {
    private:
        std::vector<std::unique_ptr<DeclVariable>> args;
        StmtPtr body;

    public:
        DeclFunction(std::string_view _name
        ,        std::vector<std::unique_ptr<DeclVariable>> _args
        ,        common::SourceLocation _name_loc = default_pos()
        ,        StmtPtr _body = nullptr
        ,        common::SourceLocation _pos = default_pos()
        ):  DeclName(_name
            ,        std::move(_name_loc)
            ,        std::move(_pos)
            )
        ,   args(std::move(_args))
        ,   body(std::move(_body))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
        
        const std::vector<std::unique_ptr<DeclVariable>>& get_args() const noexcept;
        const StmtNode* get_body() const noexcept;
    };

    class ImportStmt : public StmtNode
    {
    private:
        common::SourceLocation name_loc;
        SymbolPath path;
        bool m_is_relative;
        
    public:
        explicit ImportStmt(std::string_view _imported
        ,                   bool _relative = false
        ,                   common::SourceLocation _name_pos = default_pos()
        ,                   common::SourceLocation _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   path{.path = {std::string(_imported)}}
        ,   m_is_relative(_relative)
        ,   name_loc(_name_pos)
        {}

        explicit ImportStmt(SymbolPath _imported
        ,                   bool _relative = false
        ,                   common::SourceLocation _name_pos = default_pos()
        ,                   common::SourceLocation _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   path(std::move(_imported))
        ,   m_is_relative(_relative)
        ,   name_loc(_name_pos)
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        /**
         * @deprecated here only until remove its usage from old code, for build success
         */
        std::string_view get_imported() const noexcept;

        const SymbolPath& get_path() const noexcept;
        bool is_relative() const noexcept;

        /**
         * @brief Get the module path location in source code
         * @return common::SourceLocation 
         */
        common::SourceLocation get_name_location();

        /**
         * @brief Set the module path location in source code
         */
        void set_name_location();
    };

    class ReturnStmt : public StmtNode
    {
    private:
        ExprPtr ret_expr{nullptr};
        
    public:
        explicit ReturnStmt(ExprPtr _ret = nullptr
        ,                   common::SourceLocation _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   ret_expr(std::move(_ret))
        {}
        
        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        const ExprNode* get_ret_expr() const noexcept;
    };

    class BreakStmt : public StmtNode
    {
    public:
        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };
    
    class ContinueStmt : public StmtNode
    {
    public:
        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };
}
