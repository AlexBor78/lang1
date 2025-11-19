#pragma once

#include <ast/ast.h>
#include <memory>
#include <string_view>
#include <vector>

namespace lang::ast
{
    class BlockStmt : public StmtNode
    {
    private:
        std::vector<StmtPtr> m_body;
        
    public:
        explicit BlockStmt(Position _pos = default_pos()):
            StmtNode(std::move(_pos))
        {}

        explicit BlockStmt(
            std::vector<StmtPtr>& _body
        ,   Position _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   m_body(std::move(_body))
        {}

        explicit BlockStmt(
            std::vector<StmtPtr> _body
        ,   Position _pos = default_pos()
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
        ExprPtr cond;
        StmtPtr body;

    protected:
        StructureStmt(ExprPtr _cond
        ,             StmtPtr _body
        ,             Position _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   cond(std::move(_cond))
        ,   body(std::move(_body))
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;

        const ExprNode* get_cond() const;
        const StmtNode* get_body() const;
    };

    class IfStmt : public StructureStmt
    {
    public:
        IfStmt(ExprPtr _cond
        ,      StmtPtr _body
        ,      Position _pos = default_pos()
        ):  StructureStmt(std::move(_cond)
            ,             std::move(_body)
            ,             std::move(_pos)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class ForStmt : public StructureStmt
    {
    private:
        StmtPtr decl;
        StmtPtr incr;

    public:
        ForStmt(StmtPtr _decl
        ,       ExprPtr _cond
        ,       StmtPtr _incr
        ,       StmtPtr _body
        ,       Position _pos = default_pos()
        ):  StructureStmt(std::move(_cond)
            ,             std::move(_body)
            ,             std::move(_pos)
            )
        ,   decl(std::move(_decl))
        ,   incr(std::move(_incr))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        const StmtNode* get_decl() const;
        const StmtNode* get_incr() const;
    };

    class WhileStmt : public StructureStmt
    {
    public:
        WhileStmt(ExprPtr _cond
        ,         StmtPtr _body
        ,         Position _pos = default_pos()
        ):  StructureStmt(std::move(_cond)
            ,             std::move(_body)
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

    protected:
        DeclStmt(std::string_view _name
        ,        Position _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   name(_name)
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;

        std::string_view get_name() const noexcept;
    };

    class DeclVar : public DeclStmt
    {
    private:
        ExprPtr init_expr;

    public:
        explicit DeclVar(std::string_view _name
        ,                ExprPtr _init = nullptr
        ,                Position _pos = default_pos()
        ):  DeclStmt(_name
            ,        std::move(_pos)
            )
        ,   init_expr(std::move(_init))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        const ExprNode* get_init_expr() const noexcept;
    };

    class DeclFunc : public DeclStmt
    {
    private:
        std::vector<std::unique_ptr<DeclVar>> args;
        StmtPtr body;

    public:
        DeclFunc(std::string_view _name
        ,        std::vector<std::unique_ptr<DeclVar>>& _args
        ,        StmtPtr _body
        ,        Position _pos = default_pos()
        ):  DeclStmt(_name 
            ,        std::move(_pos)
            )
        ,   args(std::move(_args))
        ,   body(std::move(_body))
        {}

        DeclFunc(std::string_view _name
        ,        std::vector<std::unique_ptr<DeclVar>> _args
        ,        StmtPtr _body
        ,        Position _pos = default_pos()
        ):  DeclStmt(_name 
            ,        std::move(_pos)
            )
        ,   args(std::move(_args))
        ,   body(std::move(_body))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
        
        const std::vector<std::unique_ptr<DeclVar>>& get_args() const noexcept;
        const StmtNode* get_body() const noexcept;
    };

    class DeclModule : public DeclStmt
    {
    public:
        explicit DeclModule(std::string_view _name
        ,                   Position _pos = default_pos()
        ):  DeclStmt(_name
            ,        std::move(_pos)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class DeclNamespace : public DeclStmt
    {
    private:
        StmtPtr body;

    public:
        explicit DeclNamespace(std::string_view _name
        ,                      StmtPtr _body
        ,                      Position _pos = default_pos()
        ):  DeclStmt(_name
            ,        std::move(_pos)
            )
        ,   body(std::move(_body))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        const StmtNode* get_body() const noexcept;
    };

    class ImportStmt : public StmtNode
    {
    private:
        std::string imported;
        
    public:
        explicit ImportStmt(std::string_view _imported
        ,                   Position _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   imported(_imported)
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        std::string_view get_imported() const noexcept;
    };

    class ReturnStmt : public StmtNode
    {
    private:
        ExprPtr ret_expr{nullptr};
    public:
        explicit ReturnStmt(ExprPtr _ret = nullptr
        ,                   Position _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   ret_expr(std::move(_ret))
        {}
        
        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        const ExprNode* get_ret_expr() const noexcept;
    };
}
