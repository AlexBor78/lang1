#include <string>
#include <format>
#include <vector>
#include <cassert>
#include <filesystem>

#include <print>

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

    void ModulesLoader::load(const std::vector<SymbolPath>& paths) {
        for(const auto& path : paths) {
            load(path);
        }
    }

    void ModulesLoader::debug_break() {
        #ifdef MODULESLOADER_DEBUG
            common::debug_break();
        #endif
    }

    void ModulesLoader::load(std::string_view _file_path) {
        debug_break();

				const std::string file_path = std::filesystem::canonical(_file_path).string();
				auto str_id = program->dirty_strings.add(file_path);

        // check if file already processed
        if(program->targets_storage.contains(str_id)) return;
        //if(program->sources_storage.contains(file_path)) return;
        
        // process file
				program->logger.set_name("SYNTAX");
				program->logger.log("processing file: {}", file_path);
				program->targets_storage.add_path(str_id);
        auto unresolved_imports = syntax_driver.process_file(file_path);
        const auto resolved_imports = import_resolver.process_imports(unresolved_imports);

        // updating data for relative paths solving
				const auto sympath = import_resolver.gen_sympath(file_path);
				const auto workdir = std::filesystem::path(file_path).parent_path().string();

				import_resolver.set_worksympath(sympath);
        import_resolver.set_workdir(workdir);

        // recursively load all other files
        load(resolved_imports);
    }

    void ModulesLoader::load(SymbolPath sympath) {
        debug_break();

        
        // generating file paths
        const std::string _file_path = import_resolver.gen_path(sympath);
				const std::string file_path = std::filesystem::canonical(_file_path).string();
				auto str_id = program->dirty_strings.add(file_path);
				
// TODO: add load_root(file_ath) func, and move main load(file_path) logic
//				load(file_path);
//				return;
				
        // updating info for relative paths solving
				import_resolver.set_worksympath(sympath);

        // check just in case if file was already processed
        if(program->targets_storage.contains(str_id)) return;
        //if(program->sources_storage.contains(file_path)) return;

        // process files
				program->logger.set_name("SYNTAX");
				program->logger.log("processing file: {}", file_path);
				program->targets_storage.add_path(str_id);
        auto unresolved_imports = syntax_driver.process_file(file_path);

        // recursively load all other files
        const auto resolved_imports = import_resolver.process_imports(unresolved_imports);
        load(resolved_imports);
    }
}
