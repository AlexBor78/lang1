#pragma once

#include <ast/ast.h>
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
        
        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;

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
        virtual void accept(ConstVisitor&) const noexcept override = 0;
        virtual void accept(NodeVisitor&) noexcept override = 0;

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

        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;
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

        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;

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

        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;
    };

    class DeclStmt : public StmtNode
    {
    protected:
        DeclStmt(Position _pos = default_pos()):
            StmtNode(std::move(_pos))
        {}

    public:
        virtual void accept(ConstVisitor&) const noexcept override = 0;
        virtual void accept(NodeVisitor&) noexcept override = 0;
    };

    class DeclVar : public DeclStmt
    {
    private:
        ExprPtr init;

    public:
        explicit DeclVar(ExprPtr _init = nullptr
        ,                Position _pos = default_pos()
        ):  DeclStmt(std::move(_pos))
        ,   init(std::move(_init))
        {}

        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;

    };

    class DeclFunc : public DeclStmt
    {
    private:
        std::vector<StmtPtr> args;
        StmtPtr body;

    public:
        DeclFunc(std::vector<StmtPtr>& _args
        ,        StmtPtr _body
        ,        Position _pos = default_pos()
        ):  DeclStmt(std::move(_pos))
        ,   args(std::move(_args))
        ,   body(std::move(_body))
        {}

        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;
        
        const std::vector<StmtPtr>& get_args() const noexcept;
        const StmtNode* get_body() const noexcept;
    };

    class DeclModule : public DeclStmt
    {
    private:
        std::string name;
    public:
        explicit DeclModule(std::string_view _name
        ,                   Position _pos = default_pos()
        ):  DeclStmt(std::move(_pos))
        ,   name(_name)
        {}

        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;

        std::string_view get_name() const noexcept;
    };

    class DeclNamespace : public DeclStmt
    {
    private:
        std::string name;
    public:
        explicit DeclNamespace(std::string_view _name
        ,                      Position _pos = default_pos()
        ):  DeclStmt(std::move(_pos))
        ,   name(_name)
        {}

        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;

        std::string_view get_name() const noexcept;
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

        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;

        std::string_view get_imported() const noexcept;
    };

    class ReturnStmt : public StmtNode
    {
    private:
        ExprPtr ret{nullptr};
    public:
        explicit ReturnStmt(ExprPtr _ret = nullptr
        ,                   Position _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   ret(std::move(_ret))
        {}
        
        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;

        const ExprNode* get_ret() const noexcept;
    };
}
