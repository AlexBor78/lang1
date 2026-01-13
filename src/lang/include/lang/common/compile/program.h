#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include <lang/common/compile/unit.h>
#include <lang/common/compile/state.h>
#include <lang/common/compile/options.h>
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
        std::unordered_map<ast::BaseNode*, std::unique_ptr<AbstractType>> types_context;

        /**
         * @brief   list of all external symbols (vars and functoin)
         */
        std::unordered_set<ast::BaseNode*> extern_list;

        /**
         * @brief   UnitID -> list of Module export (temporary by UnitID)
         */
        std::unordered_map<UnitID, std::unordered_set<ast::BaseNode*>> export_lists;
    };

    /**
     * @brief   root structure of all data
     * @todo    store compile_options, state, semantic HERE (after removing Program from SemanticState)
     */
    struct Program {        
        /**
         * @brief   name of program, and name of output binary
         */
        std::string name{"main"};

        /**
         * @brief   compile optoins of program
         */
        CompileOptions compile_options;

        /**
         * @brief   compile state of program
         */
        CompileState compile_state;

        /**
         * @brief   container with all CompileUnits
         * @note    will replace `modules` field
         */
        CompileUnitsManager compile_units_manager;

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
