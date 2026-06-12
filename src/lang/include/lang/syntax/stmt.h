#pragma once

#include <memory>
#include <vector>
#include <string_view>
#include <lang/syntax/ast.h>
#include <lang/common/symbol_path.h>
#include <lang/semantic/types/typesystem.h>

/**
 * @todo rename _pos to _full_range_loc, all pos like to loc like.
 */
namespace lang::syntax
{   
    class BlockStmt : public StmtNode {
    private:
        std::vector<StmtPtr> m_body;
        
    public:
        explicit BlockStmt(common::SourceLocation _full_range_loc = default_pos()):
            StmtNode(std::move(_full_range_loc))
        {}

        explicit BlockStmt(
            std::vector<StmtPtr>& _body
        ,   common::SourceLocation _full_range_loc = default_pos()
        ):  StmtNode(std::move(_full_range_loc))
        ,   m_body(std::move(_body))
        {}

        explicit BlockStmt(
            std::vector<StmtPtr> _body
        ,   common::SourceLocation _full_range_loc = default_pos()
        ):  StmtNode(std::move(_full_range_loc))
        ,   m_body(std::move(_body))
        {}
        
        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        const std::vector<StmtPtr>& get_body() const noexcept;
        std::vector<StmtPtr>& get_body() noexcept;

        void add_tobody(StmtPtr) noexcept;
    };

		/**
		 * rename to ControlFlow
		 */
    class StructureStmt : public StmtNode {
    private:
        StmtPtr body;

    protected:
        explicit StructureStmt(StmtPtr _body
        ,                      common::SourceLocation _word_loc = default_pos()
        ,                      common::SourceLocation _full_range_loc = default_pos()
        ):  StmtNode(std::move(_full_range_loc))
        ,   body(std::move(_body))
        ,   keyword_loc(_word_loc)
        {}

    public:
        const StmtNode* get_body() const;
        StmtNode* get_body();

        common::SourceLocation keyword_loc;
    };

		/**
		 * @todo rename to ConditionalStmt
		 */
    class StructureStmtWithCond : public StructureStmt {
    private:
        ExprPtr cond;

    protected:
        StructureStmtWithCond(ExprPtr _cond
        ,                     StmtPtr _body
        ,                     common::SourceLocation _word_loc = default_pos()
        ,                     common::SourceLocation _full_range_loc = default_pos()
        ):  StructureStmt(std::move(_body)
            ,             std::move(_word_loc)
            ,             std::move(_full_range_loc)
            )
        ,   cond(std::move(_cond))
        {}

    public:

        const ExprNode* get_cond() const;
        ExprNode* get_cond();
    };

    class IfStmt : public StructureStmtWithCond {
    public:
        IfStmt(ExprPtr _cond
        ,                     StmtPtr _body
        ,                     common::SourceLocation _word_loc = default_pos()
        ,                     common::SourceLocation _full_range_loc = default_pos()
        ):  StructureStmtWithCond(        		
													std::move(_cond)
						,							std::move(_body)
            ,             std::move(_word_loc)
            ,             std::move(_full_range_loc)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class ElseStmt : public StructureStmt {
    public:
        ElseStmt(
        			StmtPtr _body
        ,     common::SourceLocation _word_loc = default_pos()
        ,     common::SourceLocation _full_range_loc = default_pos()
        ):  StructureStmt(std::move(_body)
            ,             std::move(_word_loc)
            ,             std::move(_full_range_loc)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class ForStmt : public StructureStmtWithCond {
    private:
        StmtPtr decl;
        StmtPtr incr;

    public:
        ForStmt(StmtPtr _decl
        ,       ExprPtr _cond
        ,       StmtPtr _incr
        ,       StmtPtr _body
        ,       common::SourceLocation _word_loc = default_pos()
        ,       common::SourceLocation _full_range_loc = default_pos()
        ):  StructureStmtWithCond(std::move(_cond)
            ,             std::move(_body)
            ,             std::move(_word_loc)
            ,             std::move(_full_range_loc)
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

    class WhileStmt : public StructureStmtWithCond {
    public:
        WhileStmt(ExprPtr _cond
        ,      		StmtPtr _body
        ,      		common::SourceLocation _word_loc = default_pos()
        ,      		common::SourceLocation _full_range_loc = default_pos()
        ):  StructureStmtWithCond(        		
													std::move(_cond)
						,							std::move(_body)
            ,             std::move(_word_loc)
            ,             std::move(_full_range_loc)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class DeclStmt : public StmtNode {
    private:
        std::string name;

    protected:
        DeclStmt(std::string_view _name
        ,        common::SourceLocation _name_loc = default_pos()
        ,        common::SourceLocation _full_range_loc = default_pos()
        ):  StmtNode(std::move(_full_range_loc))
        ,   name(_name)
        ,   name_loc(_name_loc)
        {}

    public:
        std::string_view get_name() const noexcept;
        common::SourceLocation name_loc;
    };
    
    class DeclName : public DeclStmt {
    protected:
				using DeclStmt::DeclStmt;
		public:
				std::unique_ptr<AbstractType> type;
    };

    class DeclVariable : public DeclName {
    private:
        ExprPtr init_expr;

    public:
        explicit DeclVariable(std::string_view _name
        ,                common::SourceLocation _name_loc = default_pos()
        ,                ExprPtr _init = nullptr
        ,                common::SourceLocation _full_range_loc = default_pos()
        ):  DeclName(_name
            ,        std::move(_name_loc)
            ,        std::move(_full_range_loc)
            )
        ,   init_expr(std::move(_init))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        const ExprNode* get_init_expr() const noexcept;
    };

    class DeclFunction : public DeclName {
    private:
        std::vector<std::unique_ptr<DeclVariable>> args;
        StmtPtr body;

    public:
        DeclFunction(std::string_view _name
        ,        std::vector<std::unique_ptr<DeclVariable>> _args
        ,        common::SourceLocation _name_loc = default_pos()
        ,        StmtPtr _body = nullptr
        ,        common::SourceLocation _full_range_loc = default_pos()
        ):  DeclName(_name
            ,        std::move(_name_loc)
            ,        std::move(_full_range_loc)
            )
        ,   args(std::move(_args))
        ,   body(std::move(_body))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
        
        const std::vector<std::unique_ptr<DeclVariable>>& get_args() const noexcept;
        const StmtNode* get_body() const noexcept;
    };

    class ImportStmt : public StmtNode {
    private:
        SymbolPath path;
        
    public:
        explicit ImportStmt(SymbolPath _imported
        ,                   common::SourceLocation _name_pos = default_pos()
        ,                   common::SourceLocation _full_range_loc = default_pos()
        ):  StmtNode(std::move(_full_range_loc))
        ,   path(std::move(_imported))
        ,   name_loc(_name_pos)
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        /**
         * @deprecated here only until remove its usage from old code, for build success
         */
        std::string_view get_imported() const noexcept;

        const SymbolPath& get_path() const noexcept;

        // bool is_relative() const noexcept;

        /**
         * @brief location of the "name" in source code
         */
        common::SourceLocation name_loc;
    };

    class ReturnStmt : public StmtNode {
    private:
        ExprPtr ret_expr{nullptr};
        
    public:
        explicit ReturnStmt(ExprPtr _ret = nullptr
        ,                   common::SourceLocation _full_range_loc = default_pos()
        ):  StmtNode(std::move(_full_range_loc))
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
