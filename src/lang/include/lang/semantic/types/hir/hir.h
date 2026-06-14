#pragma once

#ifdef __SIZEOF_INT128__
    typedef __int128 int128_t;
    typedef unsigned __int128 uint128_t;
#endif

#include <variant>
#include <string_view>

#include <lang/syntax/ast.h>
#include <lang/semantic/types/symbol.h>

namespace lang::semantic::hir {

	class Visitor;
	class ConstVisitor;
  class StmtNode;
  class ExprNode;
  class DeclStmt;
  class BaseNode;

	using TypeID = AbstractType*; // just for now
	using ASTPtr 	=	syntax::BaseNode*;
  using StmtPtr = StmtNode*;
  using ExprPtr = ExprNode*;
  using HIR = std::vector<BaseNode*>;
	
	class BaseNode {
	protected:
		BaseNode(
			ASTPtr _ast_node
		):	ast_node(_ast_node)
		{}
			
	public:
		virtual ~BaseNode() = default;
		ASTPtr ast_node{nullptr};
		virtual void accept(Visitor&) noexcept = 0;
		virtual void accept(ConstVisitor&) const noexcept = 0;
	};
	
  class StmtNode : public BaseNode
  {
  protected:
		using BaseNode::BaseNode;
  };

  class ExprNode : public StmtNode
  {
  protected:
		ExprNode(
			TypeID _type_id
		,	ASTPtr	_ast_node
		):	StmtNode(_ast_node)
		,		type_id(_type_id)
		{}
	public:
		TypeID type_id;
  };

  class DeclNode : public StmtNode {
  protected:
		DeclNode(
			SymbolID _symbol_id
		,	ASTPtr _ast_node	
		):	StmtNode(_ast_node)
		,		symbol_id(_symbol_id)
		{}
	public: SymbolID symbol_id;
  };
	


	


  class BlockStmt : public StmtNode {
  public:
		ScopeID scope_id;
    explicit BlockStmt(
			ASTPtr _ast
		,	ScopeID _scope_id
		):	StmtNode(_ast)
		,		scope_id(_scope_id)
    {}
    
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
  };

	class ControlFlowStmt : public StmtNode {
	protected:
			ControlFlowStmt(
				StmtPtr _body
			,	ASTPtr _ast_node
			): StmtNode(_ast_node)
			,	body(_body)
			{}

	public:
		StmtPtr body;
	};

	class ConditionalStmt : public ControlFlowStmt {
	protected:
		ConditionalStmt(
			ExprPtr _cond
		,	StmtPtr	_body
		,	ASTPtr	_ast_node
		):	ControlFlowStmt(
					_body
				,	_ast_node
				)
		,		cond(_cond)
		{}
	public:
		ExprPtr cond;
	};

	class IfStmt : public ConditionalStmt {
	public:
		IfStmt(
			ExprPtr _cond
		,	StmtPtr	_body
		,	ASTPtr	_ast_node
		):	ConditionalStmt(
					_cond
				,	_body
				,	_ast_node
				)
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};

	class ElseStmt : public ControlFlowStmt {
	public:
		ElseStmt(
			StmtPtr _body
		,	ASTPtr	_ast_node
		):	ControlFlowStmt(
					_body
				,	_ast_node
				)
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};

	class WhileStmt : public ConditionalStmt {
	public:
		WhileStmt(
			ExprPtr _cond
		,	StmtPtr	_body
		,	ASTPtr	_ast_node
		):	ConditionalStmt(
				_cond
			,	_body
			,	_ast_node
			)
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};

	class DeclVar : public DeclNode {
	public:
		ExprPtr init_expr;
		DeclVar(
			ExprPtr 	_init_expr
		,	SymbolID	_symbol_id
		,	ASTPtr		_ast_node
		):	DeclNode(
					_symbol_id
				,	_ast_node
				)
		,		init_expr(_init_expr)
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};

	class DeclFunc : public DeclNode {
	public:
		StmtPtr body;
		std::pmr::vector<DeclVar*> args;

		DeclFunc(
			StmtPtr _body
		,	std::pmr::vector<DeclVar*> _args
		,	SymbolID _symbol_id
		,	ASTPtr _ast_node
		):	DeclNode(
					_symbol_id
				,	_ast_node
				)
		,		body(_body)
		,		args(std::move(_args))
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};
	
	class ImportStmt : public StmtNode {
	public:
		ImportStmt(
			ASTPtr _ast_node
		):	StmtNode(_ast_node)
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};
	class ReturnStmt: public StmtNode {
	public: ExprPtr return_expr;
		ReturnStmt(
			ExprPtr	_return_expr
		,	ASTPtr 	_ast_node
		):	StmtNode(_ast_node)
		,		return_expr(_return_expr)
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};
	class BreakStmt : public StmtNode {
	public:
		BreakStmt(
			ASTPtr _ast_node
		):	StmtNode(_ast_node)
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};
	class ContinueStmt : public StmtNode {
	public:
		ContinueStmt(
			ASTPtr _ast_node
		):	StmtNode(_ast_node)
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};





	class LiteralExpr : public ExprNode {
	protected:
		using ExprNode::ExprNode;
	};

	class StringLiteral : public LiteralExpr {
	public:
		std::string_view literal;

		StringLiteral(
			std::string_view _literal
		,	TypeID _type_id
		,	ASTPtr	_ast_node
		):	LiteralExpr(
					_type_id
				,	_ast_node
			)
		,	literal(_literal)
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};

	class IntegerLiteral : public LiteralExpr {
	public:
		using IntegerValue = std::variant<
			int64_t		
		,	uint64_t	
		>; IntegerValue value;

		IntegerLiteral(
			IntegerValue	_value
		,	TypeID				_type_id
		,	ASTPtr				_ast_node
		):	LiteralExpr(
					_type_id
				,	_ast_node
			)
		,	value(_value)
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};

	class FloatLiteral : public LiteralExpr {
	public:
		using FloatValue = std::variant <
			float
		,	double
		>; FloatValue value;

		FloatLiteral(
			FloatValue 	_value
		,	TypeID 	 	_type_id
		,	ASTPtr			_ast_node
		):	LiteralExpr(
					_type_id
				,	_ast_node
				)
		,		value(_value)
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};

	class BoolLiteral : public LiteralExpr {
	public:
		bool value;
		BoolLiteral(
			bool 		_value
		,	TypeID _type_id
		,	ASTPtr	_ast_node
		):	LiteralExpr(
					_type_id
				,	_ast_node
				)
		,		value(_value)
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};

	class IdentifierExpr : public ExprNode{
	public: SymbolID symbol_id;
	protected:
		IdentifierExpr(
			SymbolID 	_symbol_id
		,	TypeID 	_type_id
		,	ASTPtr		_ast_node
		):	ExprNode(
					_type_id
				,	_ast_node
				)
		,		symbol_id(_symbol_id)
		{}
	};

	class VariableExpr : public IdentifierExpr {
	public:
			VariableExpr(
				SymbolID 	_symbol_id
			,	TypeID		_type_id
			,	ASTPtr		_ast_node
			):	IdentifierExpr(
						_symbol_id
					,	_type_id
					,	_ast_node
					)
			{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};

	class CallExpr : public IdentifierExpr {
	public:
		std::pmr::vector<ExprPtr> args;
		CallExpr(
			SymbolID _symbol_id
		,	TypeID	_type_id
		,	std::pmr::vector<ExprPtr> _args
		,	ASTPtr	_ast_node
		):	IdentifierExpr(
					_symbol_id
				,	_type_id
				,	_ast_node
				)
		,		args(std::move(_args))
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};
	
	class OperatorExpr : public ExprNode {
	public:	enum class Kind {
      ASSIGN,     // =
									
      // Arithmetic
      PLUS,       // +
      MINUS,      // -
      STAR,       // *
      SLASH,      // /
      PERCENT,    // %

      // Unary / Address / Dereference
      AMPERSAND,  // &
      BANG,       // !
      TILDE,      // ~

      // Increment / Decrement
      INCREMENT,  // ++
      DECREMENT,  // --

      // Comparison
      EQ,         // ==
      NEQ,        // !=
      LT,         // <
      LE,         // <=
      GT,         // >
      GE,         // >=

      // Logical
      AND,        // &&
      OR,         // ||

      // Bitwise
      BIT_OR,         // |
      // BIT_AND = AMPERSAND, // &
      BIT_XOR,        // ^
      SHIFT_LEFT,     // <<
      SHIFT_RIGHT     // >>
		};
	public: Kind op;
	protected:
		OperatorExpr(
			Kind 		_op
		,	TypeID _type_id
		,	ASTPtr	_ast_node
		):	ExprNode(
					_type_id
				,	_ast_node
			)
		,	op(_op)
		{}
	};

	class BinOpExpr : public OperatorExpr {
	public:
		ExprPtr left;
		ExprPtr	right;

		BinOpExpr(
			ExprPtr _left
		,	ExprPtr	_right
		,	Kind		_op
		,	TypeID	_type_id
		,	ASTPtr	_ast_node
		): OperatorExpr(
				_op
			,	_type_id
			,	_ast_node
			)
		,	left(_left)
		,	right(_right)
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};

	class UnaryOpExpr : public OperatorExpr {
	public: ExprPtr operand;
	protected:
		UnaryOpExpr(
			ExprPtr	_operand
		,	Kind		_op
		,	TypeID	_type_id
		,	ASTPtr	_ast_node
		):	OperatorExpr(
					_op
				,	_type_id
				,	_ast_node
				)
		,		operand(_operand)
		{}
	};

	class PrefixOpExpr : public UnaryOpExpr {
	public:
		PrefixOpExpr(
			ExprPtr	_operand
		,	Kind		_op
		,	TypeID	_type_id
		,	ASTPtr	_ast_node
		):	UnaryOpExpr(
					_operand
				,	_op
				,	_type_id
				,	_ast_node
			)
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};
	class PostfixOpExpr : public UnaryOpExpr {
	public:
		PostfixOpExpr(
			ExprPtr	_operand
		,	Kind		_op
		,	TypeID	_type_id
		,	ASTPtr	_ast_node
		):	UnaryOpExpr(
					_operand
				,	_op
				,	_type_id
				,	_ast_node
			)
		{}
    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};

	class StackAllocExpr : public ExprNode {
	public:	std::pmr::vector<size_t> dimensions;
		StackAllocExpr(
			std::pmr::vector<size_t> _dimensions
		,	TypeID	_type_id
		,	ASTPtr	_ast_node
		):	ExprNode(
					_type_id
				,	_ast_node
				)
		,		dimensions(std::move(_dimensions))
		{}

    virtual void accept(ConstVisitor&) const noexcept override;
    virtual void accept(Visitor&) noexcept override;
	};
}
