#pragma once

#include <vector>
#include <string_view>
#include <lang/syntax/ast/ast.h>
#include <lang/common/symbol_path.h>
#include <lang/semantic/types.h>

/**
 * @todo rename _pos to _full_range_loc, all pos like to loc like.
 */
namespace lang::syntax::ast
{   
    class BlockStmt : public StmtNode {
    public:
        std::vector<StmtPtr> body;
        explicit BlockStmt(
						common::SourceLocation _full_range_loc = default_pos()):
            StmtNode(std::move(_full_range_loc))
        {}

        explicit BlockStmt(
          std::vector<StmtPtr>& _body
        , common::SourceLocation _full_range_loc = default_pos()
        ):  StmtNode(std::move(_full_range_loc))
        ,   body(std::move(_body))
        {}

        explicit BlockStmt(
          std::vector<StmtPtr> _body
        , common::SourceLocation _full_range_loc = default_pos()
        ):  StmtNode(std::move(_full_range_loc))
        ,   body(std::move(_body))
        {}
        
        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

		/**
		 * @todo TODO rename to ControlFlow
		 */
    class StructureStmt : public StmtNode {
    protected:
        explicit StructureStmt(
					StmtPtr _body
        , common::SourceLocation _word_loc = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  StmtNode(std::move(_full_range_loc))
        ,   body(std::move(_body))
        ,   keyword_loc(_word_loc)
        {}

    public:
        StmtPtr body;
        common::SourceLocation keyword_loc;
    };

		/**
		 * @todo rename to ConditionalStmt
		 */
    class StructureStmtWithCond : public StructureStmt {
    protected:
        StructureStmtWithCond(
					ExprPtr _cond
        , StmtPtr _body
        , common::SourceLocation _word_loc = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  StructureStmt(
							std::move(_body)
            , std::move(_word_loc)
            , std::move(_full_range_loc)
            )
        ,   cond(std::move(_cond))
        {}

    public:
        ExprPtr cond;
    };

    class IfStmt : public StructureStmtWithCond {
    public:
        IfStmt(
					ExprPtr _cond
        , StmtPtr _body
        , common::SourceLocation _word_loc = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  StructureStmtWithCond(        		
							std::move(_cond)
						,	std::move(_body)
            , std::move(_word_loc)
            , std::move(_full_range_loc)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class ElseStmt : public StructureStmt {
    public:
        ElseStmt(
        	StmtPtr _body
        , common::SourceLocation _word_loc = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  StructureStmt(
							std::move(_body)
            , std::move(_word_loc)
            , std::move(_full_range_loc)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class ForStmt : public StructureStmtWithCond {
    public:
        StmtPtr decl;
        StmtPtr incr;

        ForStmt(
					StmtPtr _decl
        , ExprPtr _cond
        , StmtPtr _incr
        , StmtPtr _body
        , common::SourceLocation _word_loc = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  StructureStmtWithCond(
							std::move(_cond)
            , std::move(_body)
            , std::move(_word_loc)
            , std::move(_full_range_loc)
            )
        ,   decl(std::move(_decl))
        ,   incr(std::move(_incr))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class WhileStmt : public StructureStmtWithCond {
    public:
        WhileStmt(
					ExprPtr _cond
        , StmtPtr _body
        , common::SourceLocation _word_loc = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  StructureStmtWithCond(        		
							std::move(_cond)
						,	std::move(_body)
            , std::move(_word_loc)
            , std::move(_full_range_loc)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class DeclStmt : public StmtNode {
    protected:

        DeclStmt(
					std::string_view _name
        , common::SourceLocation _name_loc = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  StmtNode(std::move(_full_range_loc))
        ,   name(_name)
        ,   name_loc(_name_loc)
        {}

    public:
				bool is_export{false};
        std::string_view name;
        common::SourceLocation name_loc;
    };
    
    class DeclName : public DeclStmt {
    protected:
				using DeclStmt::DeclStmt;
		public:
				bool is_extern{false};
				semantic::AbstractType* type;
    };

    class DeclVariable : public DeclName {
    public:
        ExprPtr init_expr;

        explicit DeclVariable(
					std::string_view _name
        , common::SourceLocation _name_loc = default_pos()
        , ExprPtr _init = nullptr
        , common::SourceLocation _full_range_loc = default_pos()
        ):  DeclName(
							_name
            , std::move(_name_loc)
            , std::move(_full_range_loc)
            )
        ,   init_expr(std::move(_init))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class DeclFunction : public DeclName {
    public:
        std::pmr::vector<DeclVariable*> args;
        StmtPtr body;

        DeclFunction(
					std::string_view _name
        , std::pmr::vector<DeclVariable*> _args
        , common::SourceLocation _name_loc = default_pos()
        , StmtPtr _body = nullptr
        , common::SourceLocation _full_range_loc = default_pos()
        ):  DeclName(
							_name
            , std::move(_name_loc)
            , std::move(_full_range_loc)
            )
        ,   args(std::move(_args))
        ,   body(std::move(_body))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class ImportStmt : public StmtNode {
    public:
        SymbolPath path;
        common::SourceLocation name_loc;
        
        explicit ImportStmt(
					SymbolPath _imported
        , common::SourceLocation _name_pos = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  StmtNode(std::move(_full_range_loc))
        ,   path(std::move(_imported))
        ,   name_loc(_name_pos)
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class ReturnStmt : public StmtNode {
    public:
        ExprPtr ret_expr{nullptr};
        
        explicit ReturnStmt(
					ExprPtr _ret = nullptr
        , common::SourceLocation _full_range_loc = default_pos()
        ):  StmtNode(std::move(_full_range_loc))
        ,   ret_expr(std::move(_ret))
        {}
        
        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
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
