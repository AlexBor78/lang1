#pragma once

#include <string>

#include <common/utils/logger.h>
#include <common/memory/arena_aloc.h>

#include <lang/common/compile_options.h>

#include <lang/common/source_file.h>
#include <lang/syntax/translation_unit.h>

namespace lang
{
    /**
     * @brief   root structure of all data
     * @todo    store compile_options, state, semantic HERE (after removing Program from SemanticState)
     */
    struct Program {        
        /**
         * @brief   name of output binary
         */
        std::string name{"main"};

        CompileOptions compile_options;

				common::utils::Logger logger;

				/**
				 * @brief stores all source files till end of compilation
				 */
				syntax::SourcesStorage sources_storage; 
				
        /**
         * @brief   container with all TranslationUnit (ast basicaly)
				 * @note 		all data after syntax stage here
         */
				syntax::UnitsStorage units_storage;
    };
}
