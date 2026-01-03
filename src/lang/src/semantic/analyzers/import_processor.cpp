#include <lang/semantic/analyzers/import_analyzer.h>

namespace lang::semantic
{
    [[nodiscard("ImportAnalyzer don't save import after paring, only returns")]] 
    std::vector<ModuleID> ImportAnalyzer::process(const ast::AST& ast) {
        imports.clear();
        for(const auto& node : ast) {
            node->accept(*this);
        } return std::move(imports);
    }

    void ImportAnalyzer::set_current_path(const SymbolPath& path) {
        current_path = path;
    }

    void ImportAnalyzer::visit_import_stmt(const ast::ImportStmt& node) noexcept {
        ModuleID id;
        if(node.is_relative()) {
            // save relative path
            id.is_relative = true;
            id.relative_path = node.get_path();

            // save absolute path
            id.path = current_path;
            id.path.path.insert(
                id.path.path.end(),
                node.get_path().path.begin(),
                node.get_path().path.end()
            );
        } else id.path = node.get_path();
        
        id.path.normalize();
        imports.emplace_back(id);
    }
}
