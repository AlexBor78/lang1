#include <string>
#include <format>
#include <vector>
#include <cassert>
#include <filesystem>
#include <unordered_set>

#include <lang/utils/diagnostic.h>
#include <lang/pipeline/modules_loader.h>
#include <lang/pipeline/syntax_driver.h>

// # define MODULESLOADER_DEBUG

//constexpr const char FILE_SUFFIX[] = ".lang";
// constexpr size_t FILE_SUFFIX_SIZE = (sizeof(FILE_SUFFIX) - 1);

namespace lang::pipeline
{
    void ModulesLoader::load() {
				if(program->compile_options.inputs_files.size() == 0) {
					throw common::diagnostic::InterError("no input files provided");
				}
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
        // if(program->compile_units_manager.contains(syntax::UnitID{.filepath = file_path})) // wtf is that error?
        if(program->compile_state.processed_files.contains(file_path)) return;
        
        // process file
        auto syntax_container = syntax_driver.process_file(file_path);
        program->compile_state.processed_files.emplace(file_path);

        // creating new unit
        auto id = program->units_storage.gen_new_id();
        id.filepath = file_path;
        id.symbolpath = gen_sympath(file_path);
        program->units_storage.update_contexts(id);

        auto unit = program->units_storage.get(id);
        if(!unit) throw common::diagnostic::InterError("ModulesLoader: load_file error: UnitsManager return nullptr ");
        
        // prepare info to save
        auto dependencies = process_imports(syntax_container.imports_list);
        auto submodules = process_imports(syntax_container.submodules_list);

        assert(file_path.size() >= id.path.normalized_path.size() + program->compile_options.extension.size()
);
        // current_id = id;
        // current_path = file_path.substr(0, file_path.size() - id.path.normalized_path.size() - FILE_SUFFIX_SIZE);

        // updating data for relative paths solving
        working_sympath = id.symbolpath;        
        working_dir = file_path.substr(0, file_path.size() - id.symbolpath.absolute_path.normalized_path.size() - ext_len);
        
        // save semantic info
        program->pre_semantic_data.extern_list.insert(
            syntax_container.extern_list.begin(),
            syntax_container.extern_list.end()
        );

        // save compile unit's data
        unit->ast = std::move(syntax_container.ast);
        unit->dependencies = std::move(dependencies);
        unit->submodules = std::move(submodules);

        // recursively load all other files
        load(unit->dependencies);
        load(unit->submodules);
    }

    void ModulesLoader::load(syntax::UnitID id) {
        debug_break();
        
        // if module already loaded
        // if(program->compile_units_manager.contains(id)) return;

        // generating file paths
        std::string file_path = gen_path(id.symbolpath);

        // also save file path of compile unit
        id.filepath = file_path;
        program->units_storage.update_contexts(id);

        // updating info for relative paths solving
        // current_id = id;
        working_sympath = id.symbolpath;

        // check just in case if file was already processed
        if(program->compile_state.processed_files.contains(file_path)) return;

        // process files
        auto syntax_container = syntax_driver.process_file(file_path);
        program->compile_state.processed_files.emplace(file_path);

        // prepare info to save
        auto dependencies = process_imports(syntax_container.imports_list);
        auto submodules = process_imports(syntax_container.submodules_list);

        // save semantic info
        program->pre_semantic_data.extern_list.insert(
            syntax_container.extern_list.begin(),
            syntax_container.extern_list.end()
        );

        // save compile unit's data
        auto* unit = program->units_storage.get(id);
        unit->ast = std::move(syntax_container.ast);
        unit->dependencies = std::move(dependencies);
        unit->submodules = std::move(submodules);

        // recursively load all other files
        load(unit->dependencies);
        load(unit->submodules);
    }

    void ModulesLoader::load(const std::vector<syntax::UnitID>& units) {
        for(const auto& unit : units) {
            load(unit);
        }
    }

    // old one
    // std::vector<syntax::UnitID> ModulesLoader::process_imports(const std::unordered_set<syntax::ImportStmt*>& imports) {
    //     std::vector<semantic::ModuleID> output;
    //     for(const auto* node : imports) {
    //         semantic::ModuleID depend_id;
    //         if(node->is_relative()) {
    //             depend_id.is_relative = true;process_imports
    //             depend_id.relative_path = node->get_path();

    //             depend_id.path = current_id.path;
    //             depend_id.path.path.insert(
    //                 depend_id.path.path.end(),
    //                 node->get_path().path.begin(),
    //                 node->get_path().path.end()
    //             );
    //         } else depend_id.path = node->get_path();

    //         depend_id.path.normalize();
    //         output.emplace_back(depend_id);
    //     } return std::move(output);
    // }

    std::vector<syntax::UnitID> ModulesLoader::process_imports(const std::unordered_set<syntax::ImportStmt*>& imports) {
        std::vector<syntax::UnitID> output;
        for(const auto* node : imports) {
            // creating new unit
            syntax::UnitID id; // = program->compile_units_manager.add_new_id();

            // if it's relative path, we now can generate absolute
            if(node->get_path().is_relative) {
                auto sympath = node->get_path();
                
                // inserting root of absolute path (current module)
                sympath.absolute_path = working_sympath.absolute_path;
                sympath.absolute_path.path.insert(
                    sympath.absolute_path.path.end(),
                    sympath.relative_path.path.begin(),
                    sympath.relative_path.path.end()
                ); id.symbolpath = sympath;
            } else id.symbolpath = node->get_path();
            id.symbolpath.normalize();

            if(!program->units_storage.contains(id.symbolpath)) id.id = program->units_storage.gen_new_id().id;
            program->units_storage.update_contexts(id);

            output.emplace_back(std::move(id));
        } return output;
    }

    SymbolPath ModulesLoader::gen_sympath(const std::string& file_name) {
        assert(file_name.substr(file_name.size() - program->compile_options.extension.size()
, program->compile_options.extension.size()
) ==  program->compile_options.extension.size()
);

        std::string module_name;
        if(file_name.contains('/')) module_name = file_name.substr(file_name.find_last_of("/") + 1, file_name.size() - ext_len);
        else module_name = file_name.substr(0, file_name.size() - ext_len);
        
        SymbolPath path{
					.absolute_path = {
						.path = {module_name},
						.normalized_path = {}
					},
					.relative_path = {}
				};
        path.normalize(); // optimize

        return path;
    }

    /**
     * @brief generating path to file by symbolpath and given path start
     * 
     * @param sympath - absolute symbolpath of module 
     * @param start_path start of path
     * @return std::string 
     */
    static std::string gen_path_(
			const SymbolPath& sympath
		, std::string extension
		, std::string start_path = "./"
		) {

        if(sympath.absolute_path.empty()) {
            throw common::diagnostic::InterError("gen_path_(): needs absolute sympath to module to generate");
        }

        // creating base path
        std::string file_path = start_path;
        for(size_t i = 0; i + 1 < sympath.absolute_path.path.size(); ++i) file_path += sympath.absolute_path.path[i] + "/";
        file_path += sympath.absolute_path.path.back();

        // if it's library
        if(std::filesystem::is_directory(file_path)) {
            file_path += "/" + sympath.absolute_path.path.back() + extension;
            if(!std::filesystem::exists(file_path)) throw common::diagnostic::InterError(std::format("file {} doesn't exists", file_path));
            return file_path;
        }
        
        // just module, without submodule
        file_path += extension;
        if(!std::filesystem::exists(file_path)) throw common::diagnostic::InterError(std::format("file {} doesn't exists", file_path));
        return file_path;
    }

    std::string ModulesLoader::gen_path(const SymbolPath& sympath) {
        assert(!id.path.path.empty());

        if(sympath.is_relative) {
            return gen_path_(
							sympath
						,	program->compile_options.extension
						, working_dir
						);
        }

        for(const auto& path : program->compile_options.import_paths) {
            try {
                std::string buf = gen_path_(sympath, program->compile_options.extension, path);
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
        return gen_path_(sympath, program->compile_options.extension, working_dir);
        throw common::diagnostic::InterError(std::format("Cannot open file of {} module", sympath.absolute_path.normalize()));
    }
}
