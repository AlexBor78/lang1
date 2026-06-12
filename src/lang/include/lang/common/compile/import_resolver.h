#pragma once

#include <string>
#include <lang/common/symbol_path.h>

namespace lang {

	class ImportResolver {
	private:

		std::string working_dir;
		SymbolPath working_sympath;
		
		std::string extension;
		size_t ext_len{0};
		const std::vector<std::string>* import_paths;

	public:

		ImportResolver(
			std::string_view _extension
		,	const std::vector<std::string>* _import_paths = nullptr
		):	extension(_extension)
		,		ext_len(extension.size())
		,		import_paths(_import_paths)
		{}

		std::string gen_path(const SymbolPath&);

		inline const SymbolPath& get_worksympath() const noexcept {
			return working_sympath;
		}

		inline void set_workdir(const std::string& dir) {
			working_dir = dir;

		}
		inline void set_worksympath(const SymbolPath& path) {
			working_sympath = path;
		}


	};
}
