#pragma once

#include <string>
#include <unordered_set>
#include <common/utils/logger.h>
#include <lang/common/symbol_path.h>
#include <lang/syntax/ast/stmt.h>

namespace lang::pipeline {

	class ImportResolver {
	private:

		std::string working_dir;
		SymbolPath working_sympath;
		
		std::string extension;
		size_t ext_len{0};
		const std::vector<std::string>* import_paths;
		common::utils::Logger* logger;

	public:

		ImportResolver(
			std::string_view _extension
		,	const std::vector<std::string>* _import_paths = nullptr
		,	common::utils::Logger* _logger = nullptr
		):	extension(_extension)
		,		ext_len(extension.size())
		,		import_paths(_import_paths)
		,		logger(_logger)
		{}

		std::string gen_path(const SymbolPath&);
		std::vector<SymbolPath> process_imports(
				const std::unordered_set<syntax::ast::ImportStmt*>&
		);

		inline const SymbolPath& get_worksympath() const noexcept {
			return working_sympath;
		}

		inline void set_workdir(const std::string& dir) {
			working_dir = dir;

		}
		inline void set_worksympath(const SymbolPath& path) {
			working_sympath = path;
		}

    /**
     * @brief   generating module id by file path (file must be root file e.g. main file or inner of library)
     * @param   file_path
     * @return  semantic::ModuleID moduleID of that file
     * @warning file must be root file e.g. main file or inner of library, bcs it's just ignore's full path, and use only file_name
     * @todo    rename to show that it'is only for root files
     */
    SymbolPath gen_sympath(const std::string&);
	};
}
