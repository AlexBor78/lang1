#include <string>
#include <format>
#include <cassert>
#include <filesystem>

#include <lang/utils/diagnostic.h>
#include <lang/pipeline/modules_loader.h>
#include <lang/pipeline/syntax_driver.h>

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

    void ModulesLoader::load(const std::string& file_path) {
        common::debug_break();
        if(compile_state->processed_files.contains(file_path)) return;
        
        // process file
        auto syntax_container = syntax_driver.process_file(file_path);
        auto id = genid(file_path);
        import_processor.set_current_path(id.path);
        auto dependencies = import_processor.process(syntax_container.ast);
        compile_state->processed_files.emplace(file_path);

        assert(file_name.size() >= id.path.normalized_path.size() + FILE_SUFFIX_SIZE);
        current_path = file_path.substr(0, file_path.size() - id.path.normalized_path.size() - FILE_SUFFIX_SIZE);

        // save semantic info
        semantic_state->context.extern_list.insert(
            syntax_container.extern_list.begin(),
            syntax_container.extern_list.end()
        );
        semantic_state->program.modules[id] = semantic::Module::create(
            id.path,
            semantic_state->program.global_scope.get(),
            std::move(syntax_container.ast),
            std::move(dependencies)
        ); load(semantic_state->program.modules[id]->dependencies);
    }

    void ModulesLoader::load(const semantic::ModuleID& id) {
        common::debug_break();
        // if module already loaded
        if(semantic_state->program.modules.contains(id)) return;

        // generating file paths
        std::string file_path = gen_path(id);

        // check just in case if file was already processed
        if(compile_state->processed_files.contains(file_path)) return;

        // process files
        auto syntax_container = syntax_driver.process_file(file_path);
        import_processor.set_current_path(id.path);
        auto dependencies = import_processor.process(syntax_container.ast);
        
        // save semantic info
        semantic_state->context.extern_list.insert(
            syntax_container.extern_list.begin(),
            syntax_container.extern_list.end()
        );
        semantic_state->program.modules[id] = semantic::Module::create(
            id.path,
            semantic_state->program.global_scope.get(),
            std::move(syntax_container.ast),
            std::move(dependencies)
        ); load(semantic_state->program.modules[id]->dependencies);
    }

    void ModulesLoader::load(const std::vector<semantic::ModuleID>& modules) {
        for(const auto& module_ : modules) {
            load(module_);
        }
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

        assert(!id.path.path.empty());
        for(size_t i = 0; i < id.path.path.size() - 1; ++i) file_path += id.path.path[i] + "/";
            
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

        if(import_paths) for(const auto path : *import_paths) {
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

        throw common::diagnostic::InterError(std::format("Cannot open file of {} ", id.path.normalize()));
    }
}
