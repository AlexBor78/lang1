#pragma once

#include <string>
#include <vector>

#include <lang/common/symbol_path.h>
#include <lang/common/compile_options.h>
#include <lang/common/import_resolver.h>
#include <lang/pipeline/syntax_driver.h>

// constexpr const char FILE_SUFFIX[] = ".lang";
// constexpr size_t FILE_SUFFIX_SIZE = (sizeof(FILE_SUFFIX) - 1);

namespace lang::pipeline 
{
    /**
     * @brief recursively loads all imported modules from root files
     */
    class ModulesLoader
    {
    public: // api
        
        explicit ModulesLoader(Program* _program
        ):  program(_program)
				,		ext_len(program->compile_options.extension.size())
        ,   syntax_driver(_program)
				,		import_resolver(
								program->compile_options.extension
						,		&program->compile_options.import_paths
						,		&program->logger
						)
        {}

        void load();

    private: // vars
        Program* program;
				const size_t ext_len{0};
        SyntaxDriver syntax_driver;
				ImportResolver import_resolver;
				std::unordered_map<SymbolPath, std::string> paths_cache;

    private: // api
        void debug_break();
        /**
         * @brief   load files, accept only root files e.g. main file or main-inner of library
         * @param   file_path
         * @warning file must be root file e.g. main file or inner of library
         */
        void load(std::string_view);
        void load(
						SymbolPath
			//	,		syntax::UnitID
				);
        void load(const std::vector<SymbolPath>&);

    };
}
