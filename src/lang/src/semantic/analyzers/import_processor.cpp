#include <lang/semantic/analyzers/import_analyzer.h>

namespace lang::semantic
{
    [[nodiscard("ImportAnalyzer don't save import after paring, only returns")]] 
    std::vector<std::string> ImportAnalyzer::process(const ast::AST& ast) {
        imports.clear();
        for(const auto& node : ast) {
            node->accept(*this);
        } return std::move(imports);
    }

    void ImportAnalyzer::visit_import_stmt(const ast::ImportStmt& node) noexcept {
        imports.emplace_back(node.get_imported());
    }
}
