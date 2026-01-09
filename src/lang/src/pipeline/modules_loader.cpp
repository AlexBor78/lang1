#include <memory>
#include <string>
#include <format>
#include <cassert>
#include <filesystem>

#include <lang/utils/diagnostic.h>
#include <lang/pipeline/modules_loader.h>
#include <lang/pipeline/syntax_driver.h>
#include <vector>

// # define MODULESLOADER_DEBUG

/**
 * @todo rewrite that shitcode 
 */

namespace lang::pipeline
{
    void ModulesLoader::load() {
        for(const auto file : compile_options->inputs_files) {
            load(file);
        }
    }

    void ModulesLoader::debug_break() {
        #ifdef MODULESLOADER_DEBUG
            common::debug_break();
        #endif
    }

    void ModulesLoader::load(const std::string& file_path) {
        debug_break();
        if(compile_state->processed_files.contains(file_path)) return;
        
        // process file
        auto syntax_container = syntax_driver.process_file(file_path);
        compile_state->processed_files.emplace(file_path);
        auto id = genid(file_path);
        
        // auto dependencies = syntax_container.imports_list;
        auto dependencies = process_imports(syntax_container.imports_list);
        auto submodules = process_imports(syntax_container.submodules_list);

        assert(file_path.size() >= id.path.normalized_path.size() + FILE_SUFFIX_SIZE);
        current_id = id;
        current_path = file_path.substr(0, file_path.size() - id.path.normalized_path.size() - FILE_SUFFIX_SIZE);

        // save semantic info
        semantic_state->context.extern_list.insert(
            syntax_container.extern_list.begin(),
            syntax_container.extern_list.end()
        );
        semantic_state->program.modules[id] = std::make_unique<semantic::Module>(
            id,
            std::move(syntax_container.ast),
            std::move(dependencies),
            std::move(submodules),
            std::move(syntax_container.export_list)
        ); 
        load(semantic_state->program.modules[id]->dependencies);
        load(semantic_state->program.modules[id]->submodules);
    }

    void ModulesLoader::load(const semantic::ModuleID& id) {
        debug_break();
        
        // if module already loaded
        if(semantic_state->program.modules.contains(id)) return;

        // generating file paths
        std::string file_path = gen_path(id);
        current_id = id;

        // check just in case if file was already processed
        if(compile_state->processed_files.contains(file_path)) return;

        // process files
        auto syntax_container = syntax_driver.process_file(file_path);
        compile_state->processed_files.emplace(file_path);
        auto dependencies = process_imports(syntax_container.imports_list);
        auto submodules = process_imports(syntax_container.submodules_list);

        // save semantic info
        semantic_state->context.extern_list.insert(
            syntax_container.extern_list.begin(),
            syntax_container.extern_list.end()
        );
        semantic_state->program.modules[id] = std::make_unique<semantic::Module>(
            id,
            std::move(syntax_container.ast),
            std::move(dependencies),
            std::move(submodules),
            std::move(syntax_container.export_list)
        ); 
        load(semantic_state->program.modules[id]->dependencies);
        load(semantic_state->program.modules[id]->submodules);
    }

    void ModulesLoader::load(const std::vector<semantic::ModuleID>& modules) {
        for(const auto& module_ : modules) {
            load(module_);
        }
    }

    std::vector<semantic::ModuleID> ModulesLoader::process_imports(const std::unordered_set<ast::ImportStmt*>& imports) {
        std::vector<semantic::ModuleID> output;
        for(const auto* node : imports) {
            semantic::ModuleID depend_id;
            if(node->is_relative()) {
                depend_id.is_relative = true;
                depend_id.relative_path = node->get_path();

                depend_id.path = current_id.path;
                depend_id.path.path.insert(
                    depend_id.path.path.end(),
                    node->get_path().path.begin(),
                    node->get_path().path.end()
                );
            } else depend_id.path = node->get_path();

            depend_id.path.normalize();
            output.emplace_back(depend_id);
        } return std::move(output);
    }

    semantic::ModuleID ModulesLoader::genid(const std::string& file_name) {
        assert(file_name.substr(file_name.size() - FILE_SUFFIX_SIZE, FILE_SUFFIX_SIZE) ==  FILE_SUFFIX);

        std::string module_name;
        if(file_name.contains('/')) module_name = file_name.substr(file_name.find_last_of("/") + 1, file_name.size() - FILE_SUFFIX_SIZE);
        else module_name = file_name.substr(0, file_name.size() - FILE_SUFFIX_SIZE);
        
        semantic::ModuleID id;
        id.path = SymbolPath{.path = {module_name}};
        id.path.normalize(); // optimize
        // id.make_hash; // not implemented yet

        return id;
    }

    static std::string gen_path_(const semantic::ModuleID& id, std::string start_path) {
        std::string file_path = start_path;

        if(id.path.path.empty()) {
            throw common::diagnostic::InterError("gen_path_(): path is empty");
        }

        for(size_t i = 0; i + 1 < id.path.path.size(); ++i) file_path += id.path.path[i] + "/";
            
        file_path += id.path.path.back();

        // if it's library
        if(std::filesystem::is_directory(file_path)) {
            file_path += "/" + id.path.path.back() + FILE_SUFFIX;
            if(!std::filesystem::exists(file_path)) throw common::diagnostic::InterError(std::format("file {} doesn't exists", file_path));
            return file_path;
        }
        
        // just module, without submodule
        file_path += FILE_SUFFIX;
        if(!std::filesystem::exists(file_path)) throw common::diagnostic::InterError(std::format("file {} doesn't exists", file_path));
        return file_path;
    }

    std::string ModulesLoader::gen_path(const semantic::ModuleID& id) {
        assert(!id.path.path.empty());

        if(id.is_relative) {
            return gen_path_(id, current_path);
        }

        for(const auto path : compile_options->import_paths) {
            try {
                std::string buf = gen_path_(id, path);
                return buf;
            } catch(const common::diagnostic::InterError& e) {
                continue;
            }
            // lets better fall in case of unexpected error
            // catch(...) {
            //     break;
            // }
        } 
        
        // // not sure should i try relative path, if absolute don't work
        return gen_path_(id, current_path);
        throw common::diagnostic::InterError(std::format("Cannot open file of {} module", id.path.normalize()));
    }
}
