#pragma once

#include <common/memory/pool_alloc.h>
#include <common/utils/strings_storage.h>
#include <common/utils/logger.h>

#include <lang/common/compile_options.h>
#include <lang/common/targets_storage.h>
#include <lang/semantic/global_storage.h>

namespace lang
{
    /**
     * @brief   root structure of all data
     * @todo    store compile_options, pools, all data
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
				{}

				/**
				 * @brief allocate root's object e.g. Program itself, all it's fields
				 * @note	allocating in root_pool, while constructing
				 */
				common::memory::AsyncPoolAlloc global_pool;

        CompileOptions compile_options;

				common::utils::Logger logger;

				// for all strings, that wont be in output program
				// e.g. filepaths, etc
				StringsStorage dirty_strings{&global_pool};

				TargetsStorage targets_storage{&global_pool};

				// for all program strings
				// e.g. symbols names, strings literals(parsed), etc
				common::memory::PoolAlloc strings_pool{global_pool.get_resource()};
				StringsStorage strings_storage{&global_pool};

				common::memory::PoolAlloc global_semantic_data_pool{global_pool.get_resource()};
				semantic::GlobalStorage global_semantic_storage{&global_pool};
    };
}
