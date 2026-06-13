#include <string>
#include <format>
#include <vector>
#include <cassert>
#include <filesystem>
#include <unordered_set>

#include <lang/utils/diagnostic.h>
#include <lang/pipeline/modules_loader.h>
#include <lang/pipeline/syntax_driver.h>

namespace lang::pipeline
{
    void ModulesLoader::load() {
				if(program->compile_options.inputs_files.size() == 0)
					throw common::diagnostic::InterError("no input files provided");
				
        for(const auto& file : program->compile_options.inputs_files) {
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

        // check if file already processed
        if(program->sources_storage.contains(file_path)) return;
        
        // process file
				program->logger.set_name("SYNTAX");
				program->logger.log("processing file: {}", file_path);
        auto syntax_container = syntax_driver.process_file(file_path);

        // creating new unit
        auto id = program->units_storage.gen_new_id();
        auto unit = program->units_storage.get(id);
        if(!unit) throw common::diagnostic::InterError(
						"ModulesLoader: load_file(file_path) error: UnitsManager returned nullptr"
				);

        // save compile unit's data
        unit->ast = std::move(syntax_container.ast);

        // updating data for relative paths solving
				const auto sympath = import_resolver.gen_sympath(file_path);
				import_resolver.set_worksympath(sympath);
        import_resolver.set_workdir(file_path.substr(0, 
					file_path.size() - sympath.absolute_path.normalized_path.size() - ext_len
				));

        // recursively load all other files
        const auto dependencies = import_resolver.process_imports(syntax_container.imports_list);
        const auto submodules = import_resolver.process_imports(syntax_container.submodules_list);
        load(dependencies);
        load(submodules);
    }

    void ModulesLoader::load(SymbolPath sympath) {
        debug_break();
        
        // generating file paths
        const std::string file_path = import_resolver.gen_path(sympath);

// TODO: add load_root(file_ath) func, and move main load(file_path) logic
//				load(file_path);
//				return;
				
        // updating info for relative paths solving
				import_resolver.set_worksympath(sympath);

        // check just in case if file was already processed
        if(program->sources_storage.contains(file_path)) return;

        // process files
				program->logger.set_name("SYNTAX");
				program->logger.log("processing file: {}", file_path);
        auto syntax_container = syntax_driver.process_file(file_path);

        // creating new unit
        auto id = program->units_storage.gen_new_id();
        auto* unit = program->units_storage.get(id);
        if(!unit) throw common::diagnostic::InterError(
						"ModulesLoader: load_file(file_path) error: UnitsManager returned nullptr"
				);
				
        // save compile unit's data
        unit->ast = std::move(syntax_container.ast);

        // recursively load all other files
        const auto dependencies = import_resolver.process_imports(syntax_container.imports_list);
        const auto submodules = import_resolver.process_imports(syntax_container.submodules_list);
        load(dependencies);
        load(submodules);
    }

    void ModulesLoader::load(const std::vector<SymbolPath>& paths) {
        for(const auto& path : paths) {
            load(path);
        }
    }


}
