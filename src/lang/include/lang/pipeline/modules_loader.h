#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include <lang/pipeline/syntax_driver.h>
#include <lang/common/compile/state.h>
#include <lang/common/compile/options.h>
#include <lang/semantic/types/semantic_types.h>

constexpr const char FILE_SUFFIX[] = ".lang";
constexpr size_t FILE_SUFFIX_SIZE = (sizeof(FILE_SUFFIX) - 1);

namespace lang::pipeline 
{
    /**
     * @brief loads all modules included from main files
     * @todo  rewrite implementation - right now it is shit
     */
    class ModulesLoader
    {
    public: // api
        /**
        * @brief Construct a new Modules Loader object
        * 
        * @param _compile_options 
        * @param _compile_state 
        * @param _semantic_state 
        * @param _paths 
        * @todo remove _paths param, bcs it is in compile options already
        */
        explicit ModulesLoader(const CompileOptions* _compile_options
        ,                      CompileState* _compile_state
        ,                      semantic::SemanticState* _semantic_state
        ):  compile_options(_compile_options)
        ,   compile_state(_compile_state)
        ,   semantic_state(_semantic_state)
        ,   syntax_driver(
                compile_options,
                compile_state,
                semantic_state
        )
        {}

        void load();

    private: // vars
        const CompileOptions* compile_options{nullptr};
        CompileState* compile_state{nullptr};
        semantic::SemanticState* semantic_state{nullptr};

        std::string current_path;
        semantic::ModuleID current_id;
        // semantic::ImportAnalyzer import_processor;
        SyntaxDriver syntax_driver;

    private: // api
        /**
         * @brief load files, accept only root files e.g. main file or inner of library
         * @param file_path
         * @warning file must be root file e.g. main file or inner of library
         */
        void load(const std::string&);
        void load(const semantic::ModuleID&);
        void load(const std::vector<semantic::ModuleID>&);

        std::vector<semantic::ModuleID> process_imports(const std::unordered_set<ast::ImportStmt*>&);

        /**
         * @brief generating module id by file path (file must be root file e.g. main file or inner of library)
         * @param file_path
         * @return semantic::ModuleID moduleID of that file
         * @warning file must be root file e.g. main file or inner of library, bcs it's just ignore's full path, and use only file_name
         */
        semantic::ModuleID genid(const std::string&);
        std::string gen_path(const semantic::ModuleID&);

        void debug_break();
    };
}
