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
     * @brief recursively loads all imported modules from root files
     */
    class ModulesLoader
    {
    public: // api
        
        explicit ModulesLoader(Program* _program
        ):  program(_program)
        ,   syntax_driver(_program)
        {}

        void load();

    private: // vars

        /**
         * @brief   all main data is store here
         * 
         */
        Program* program;

        /**
         * @brief   path to dir we are working in
         */
        std::string working_dir;

        /**
         * @brief   sympath to module we are working on
         */
        SymbolPath  working_sympath;
        
        SyntaxDriver syntax_driver;

    private: // api
        /**
         * @brief   load files, accept only root files e.g. main file or main-inner of library
         * @param   file_path
         * @warning file must be root file e.g. main file or inner of library
         */
        void load(const std::string&);
        void load(UnitID);
        void load(const std::vector<UnitID>&);

        std::vector<UnitID> process_imports(const std::unordered_set<ast::ImportStmt*>&);

        /**
         * @brief   generating module id by file path (file must be root file e.g. main file or inner of library)
         * @param   file_path
         * @return  semantic::ModuleID moduleID of that file
         * @warning file must be root file e.g. main file or inner of library, bcs it's just ignore's full path, and use only file_name
         * @todo    rename to show that it'is only for root files
         */
        SymbolPath gen_sympath(const std::string&);

        /**
         * @brief   generating file path from SymbolPath
         * @return  std::string 
         */
        std::string gen_path(const SymbolPath&);

        void debug_break();
    };
}
