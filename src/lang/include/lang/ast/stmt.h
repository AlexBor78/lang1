#pragma once

#include <memory>
#include <vector>
#include <string_view>
#include <lang/ast/ast.h>

namespace lang::ast
{   
    class BlockStmt : public StmtNode
    {
    private:
        std::vector<StmtPtr> m_body;
        
    public:
        explicit BlockStmt(SourceLocation _pos = default_pos()):
            StmtNode(std::move(_pos))
        {}

        explicit BlockStmt(
            std::vector<StmtPtr>& _body
        ,   SourceLocation _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   m_body(std::move(_body))
        {}

        explicit BlockStmt(
            std::vector<StmtPtr> _body
        ,   SourceLocation _pos = default_pos()
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

    protected:
        explicit StructureStmt(StmtPtr _body
        ,                      SourceLocation _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   body(std::move(_body))
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;

        const StmtNode* get_body() const;
        StmtNode* get_body();
    };

    class StructureStmtWithCond : public StructureStmt
    {
    private:
        ExprPtr cond;

    protected:
        StructureStmtWithCond(ExprPtr _cond
        ,                     StmtPtr _body
        ,                     SourceLocation _pos = default_pos()
        ):  StructureStmt(std::move(_body)
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
        ,      SourceLocation _pos = default_pos()
        ):  StructureStmtWithCond(std::move(_cond)
            ,                     std::move(_body)
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
        ,                 SourceLocation _pos = default_pos()
        ):  StructureStmt(std::move(_body)
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
        ,       SourceLocation _pos = default_pos()
        ):  StructureStmtWithCond(std::move(_cond)
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

    class WhileStmt : public StructureStmtWithCond
    {
    public:
        WhileStmt(ExprPtr _cond
        ,         StmtPtr _body
        ,         SourceLocation _pos = default_pos()
        ):  StructureStmtWithCond(std::move(_cond)
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
        ,        SourceLocation _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   name(_name)
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;

        std::string_view get_name() const noexcept;
    };

    class DeclName : public DeclStmt
    {
    private:
        QualType type;
        bool it_extern;

    protected:
        DeclName(std::string_view _name
        ,        QualType _type
        ,        bool _extern = false
        ,        SourceLocation _pos = default_pos()
        ):  DeclStmt(_name
            ,        std::move(_pos))
        ,   type(_type)
        ,   it_extern(_extern)
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
        ,                QualType _type
        ,                ExprPtr _init = nullptr
        ,                bool _extern = false
        ,                SourceLocation _pos = default_pos()
        ):  DeclName(_name
            ,        _type
            ,        _extern
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
        bool it_extern{false};
        StmtPtr body;

    public:
        DeclFunction(std::string_view _name
        ,        QualType _type
        ,        std::vector<std::unique_ptr<DeclVariable>> _args = {}
        ,        StmtPtr _body = nullptr
        ,        bool _extern = false
        ,        SourceLocation _pos = default_pos()
        ):  DeclName(_name
            ,        _type
            ,        _extern
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

    class DeclModule : public DeclStmt
    {
    public:
        explicit DeclModule(std::string_view _name
        ,                   SourceLocation _pos = default_pos()
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
        ,                      SourceLocation _pos = default_pos()
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
        ,                   SourceLocation _pos = default_pos()
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
        ,                   SourceLocation _pos = default_pos()
        ):  StmtNode(std::move(_pos))
        ,   ret_expr(std::move(_ret))
        {}
        
        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        const ExprNode* get_ret_expr() const noexcept;
    };

// TODO: 
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
