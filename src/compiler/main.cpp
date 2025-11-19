#include "ast/ast.h"
#include <memory>
#include <print>
#include <stream.h>
#include <tokenizer.h>
#include <utils/utils.h>

#include <ast/stmt.h>
#include <ast/expr.h>
#include <vector>

std::vector<std::unique_ptr<lang::ast::BaseNode>> build_ast()
{
    std::vector<std::unique_ptr<lang::ast::BaseNode>> ast;
/*
module main;
import std;
*/
    ast.emplace_back(std::make_unique<lang::ast::DeclModule>("main"));
    ast.emplace_back(std::make_unique<lang::ast::ImportStmt>("std"));

/*
namespace math {
    int add(int a, int b) {
        return a + b;
    }
}
*/

    std::vector<lang::ast::StmtPtr> math_body;

    std::vector<lang::ast::StmtPtr> add_body;
    std::vector<std::unique_ptr<lang::ast::DeclVar>> add_args;

    add_args.emplace_back(std::make_unique<lang::ast::DeclVar>("a"));
    add_args.emplace_back(std::make_unique<lang::ast::DeclVar>("b"));

    add_body.emplace_back(std::make_unique<lang::ast::ReturnStmt>(
        std::make_unique<lang::ast::BinOpExpr>(
            lang::ast::OperatorExpr::OperatorKind::PLUS,
            std::make_unique<lang::ast::VariableExpr>("a"),
            std::make_unique<lang::ast::VariableExpr>("b")
        )
    ));

    math_body.emplace_back(std::make_unique<lang::ast::DeclFunc>(
        "add",
        std::move(add_args),
        std::make_unique<lang::ast::BlockStmt>(std::move(add_body))
    ));

    ast.emplace_back(std::make_unique<lang::ast::DeclNamespace>(
        "math",
        std::make_unique<lang::ast::BlockStmt>(std::move(math_body))
    ));

/*
int main() {
    std::println("Hello, World!");
    std::println("5 + 7 = {}", math::add(5, 7));
    return 0;
}
*/

    std::vector<lang::ast::StmtPtr> main_body;
    std::vector<std::unique_ptr<lang::ast::DeclVar>> main_args;

    std::vector<lang::ast::ExprPtr> println1_args;
    println1_args.emplace_back(std::make_unique<lang::ast::StringLiteral>("Hello, World!"));
    main_body.emplace_back(std::make_unique<lang::ast::NamespaceExpr>(
        "std"
        ,   std::make_unique<lang::ast::CallExpr>(
                "println"
            ,   std::move(println1_args)
    )));

    std::vector<lang::ast::ExprPtr> println2_args;
    std::vector<lang::ast::ExprPtr> add1_args;
    
    println2_args.emplace_back(std::make_unique<lang::ast::StringLiteral>("5 + 7 = {}"));
    add1_args.emplace_back(std::make_unique<lang::ast::NumberLiteral>("5"));
    add1_args.emplace_back(std::make_unique<lang::ast::NumberLiteral>("7"));
    println2_args.emplace_back(std::make_unique<lang::ast::NamespaceExpr>(
        "math",
        std::make_unique<lang::ast::CallExpr>(
            "add",
            std::move(add1_args)
        )
    ));

    main_body.emplace_back(std::make_unique<lang::ast::NamespaceExpr>(
        "std"
        ,   std::make_unique<lang::ast::CallExpr>(
                "println"
            ,   std::move(println2_args)
    )));
    
    main_body.emplace_back(std::make_unique<lang::ast::ReturnStmt>(
        std::make_unique<lang::ast::NumberLiteral>("0")
    ));

    ast.emplace_back(std::make_unique<lang::ast::DeclFunc>(
        "main",
        std::move(main_args),
        std::make_unique<lang::ast::BlockStmt>(std::move(main_body))
    ));

    return std::move(ast);
}

int main()
{
    lang::FileStream stream("main.lang");

    std::print("Tokenizing...");
    lang::Tokenizer tokenizer(stream);
    auto tokens = tokenizer.tokenize(stream);
    std::println("Success!");
    lang::utils::print(tokens);

    auto ast = build_ast();
    std::println("\n\n\n");
    lang::utils::print(ast);
}
