#pragma once

#include <string>
#include <vector>

#include <lang/pipeline/syntax_driver.h>
#include <lang/pipeline/compile_state.h>
#include <lang/pipeline/compile_options.h>
#include <lang/semantic/types/semantic_types.h>
#include <lang/semantic/analyzers/import_analyzer.h>

constexpr const char FILE_SUFFIX[] = ".lang";
constexpr size_t FILE_SUFFIX_SIZE = (sizeof(FILE_SUFFIX) - 1);

namespace lang::pipeline {
    class ModulesLoader
    {
    public: // api
        explicit ModulesLoader(const CompileOptions* _compile_options
        ,                      CompileState* _compile_state
        ,                      semantic::SemanticState* _semantic_state
        ,                      const std::vector<std::string>* _paths = nullptr
        ):  compile_options(_compile_options)
        ,   compile_state(_compile_state)
        ,   semantic_state(_semantic_state)
        ,   import_paths(_paths)
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
        const std::vector<std::string>* import_paths{nullptr};

        std::string current_path;
        semantic::ImportAnalyzer import_processor;
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

        /**
         * @brief generating module id by file path (file must be root file e.g. main file or inner of library)
         * @param file_path
         * @return semantic::ModuleID moduleID of that file
         * @warning file must be root file e.g. main file or inner of library, bcs it's just ignore's full path, and use only file_name
         */
        semantic::ModuleID genid(const std::string&);
        std::string gen_path(const semantic::ModuleID&);
    };
}
