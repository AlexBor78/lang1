#pragma once

#include <string>

#include <common/memory/pool_alloc.h>
#include <common/utils/logger.h>
#include <common/utils/strings_storage.h>

#include <lang/common/compile_options.h>
#include <lang/common/compile_target.h>

#include <lang/syntax/translation_unit.h>
#include <lang/common/source_file.h>

namespace lang
{
    /**
     * @brief   root structure of all data
     * @todo    store compile_options, pools, all data
		 * @note		every shit by ptr bcs allocated on pool
     */
    struct Program {
			private:
				std::unique_ptr<common::memory::IPoolAlloc> root_alloc;
				
			public:
				Program(
					CompileOptions _options
				,	std::unique_ptr<common::memory::IPoolAlloc> _root_alloc
				):	root_alloc(std::move(_root_alloc))
				,		global_pool(root_alloc->get_resource())
				,		compile_options(std::move(_options))
				,		dirty_strings(&global_pool)
				,		targets_storage(&global_pool)
				{}

				/**
				 * @brief allocate root's object e.g. Program itself, all it's fields
				 * @note	allocating in root_pool, while constructing
				 */
				common::memory::AsyncPoolAlloc global_pool;

        /**
         * @brief   name of output binary
         */
 //       std::pmr::string name{"main"};
        CompileOptions compile_options;

				// std::mutex print_mutex;
				common::utils::Logger logger;

				// for all strings, that wont be in output program
				// e.g. filepaths, etc
				StringsStorage dirty_strings;

				TargetsStorage targets_storage;

				// for all program strings
				// e.g. symbols names, strings literals(parsed), etc
				StringsStorage* strings_storage{nullptr};
				common::memory::PoolAlloc* strings_pool{nullptr};


				// depreceted stuff down 

				/**
				 * @brief stores all source files till end of compilation
				 */
//				syntax::SourcesStorage sources_storage;
				
        /**
         * @brief   container with all TranslationUnit (ast basicaly)
				 * @note 		all data after syntax stage here
         */
//				syntax::UnitsStorage units_storage;

    };
}
