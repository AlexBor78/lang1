#include <filesystem>
#include <common/diagnostic/diagnostic.h>

#include <lang/common/compile/import_resolver.h>

namespace lang {
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


    std::string ImportResolver::gen_path(const SymbolPath& sympath) {
//        assert(!id.path.path.empty());

        if(sympath.is_relative) {
            return gen_path_(
							sympath
						,	extension
						, working_dir
						);
        }

        for(const auto& path : *import_paths) {
            try {
                std::string buf = gen_path_(sympath, extension, path);
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
        return gen_path_(sympath, extension, working_dir);
        throw common::diagnostic::InterError(std::format("Cannot open file of {} module", sympath.absolute_path.normalize()));
    }

    std::vector<SymbolPath> ImportResolver::process_imports(
				const std::unordered_set<syntax::ImportStmt*>& imports) {
        std::vector<SymbolPath> output;
        for(const auto* node : imports) {
						SymbolPath sympath;

            // if it's relative path, we now can generate absolute
            if(node->get_path().is_relative) {
                sympath = node->get_path();
                
                // inserting root of absolute path (current module)
                sympath.absolute_path = working_sympath.absolute_path;
                //sympath.absolute_path = working_sympath.absolute_path;
                sympath.absolute_path.path.insert(
                    sympath.absolute_path.path.end(),
                    sympath.relative_path.path.begin(),
                    sympath.relative_path.path.end()
                );
            } else sympath = node->get_path();
            sympath.normalize();
            output.emplace_back(std::move(sympath));
        } return output;
    }

    SymbolPath ImportResolver::gen_sympath(const std::string& file_name) {
//        assert(file_name.substr(file_name.size() - program->compile_options.extension.size()
//				, program->compile_options.extension.size()
//				) ==  program->compile_options.extension.size()
//				);

        std::string module_name;
        if(file_name.contains('/')) module_name = 
					file_name.substr(file_name.find_last_of("/") + 1, file_name.size() - ext_len);
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


}
