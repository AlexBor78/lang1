#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include <common/utils/logger.h>
#include <common/memory/arena_aloc.h>

#include <lang/syntax/translation_unit.h>
#include <lang/common/compile/state.h>
#include <lang/common/compile/options.h>

#include <lang/common/compile/source_file.h>

#include <lang/semantic/types/typesystem.h> // needs by PreSemanticContainer
#include <lang/semantic/types/semantic_types.h>



// temporary here util remove deprecated
// #include <lang/semantic/types/scope.h>
// #include <lang/semantic/types/module.h>
// #include <lang/semantic/types/identifier.h>

namespace lang
{
    /**
     * @brief place to temporary store semantic data after syntax stage
     */
    struct PreSemanticContainer
    {
        /**
         * @brief   DeclNode of something -> It's type
         */
        std::unordered_map<syntax::BaseNode*, std::unique_ptr<AbstractType>> types_context;

        /**
         * @brief   list of all external symbols (vars and functoin)
         */
        std::unordered_set<syntax::BaseNode*> extern_list;

        /**
         * @brief   UnitID -> list of Module export (temporary by UnitID)
         */
//        std::unordered_map<UnitID, std::unordered_set<syntax::BaseNode*>> export_lists;
    };

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

        CompileState compile_state;

				common::utils::Logger logger;

				/**
				 * @brief stores all source files till end of compilation
				 */
				syntax::SourcesStorage sources_storage; 
				
				std::unique_ptr<common::memory::ArenaAloc> ast_arena;

        /**
         * @brief   container with all TranslationUnit (ast basicaly)
				 * @note 		all data after syntax stage here
         */
				syntax::UnitsStorage units_storage;
				// syntax::UnitsStorage units_storage;


       /**
        * @brief   temporary store semantic data until it will be processed by semantic
        */
       PreSemanticContainer pre_semantic_data;

        /**
         * @brief   semantic stat
         */
        semantic::SemanticState semantic_states;

        // only deprecated stuff down

        // /**
        //  * @brief container with all modules
        //  * @deprecated used deprecated type Module - witch is combination of CompileUnit and semantic::ModuleSym
        //  */
        // std::unordered_map<semantic::ModuleID, std::unique_ptr<semantic::Module>> modules;

        // /**
        //  * @brief global scope of program
        //  * @note here stored built-in types, etc
        //  * @deprecated will be moved to GlobalScope container
        //  */
        // std::unique_ptr<semantic::Scope> global_scope{nullptr};
    };
}
