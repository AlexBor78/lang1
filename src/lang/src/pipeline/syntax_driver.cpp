#include <memory>
#include <format>
#include <string>

// syntax
#include <common/streams/istream.h>
#include <lang/syntax/lexer.h>
#include <lang/syntax/parser.h>

// semantic data container
#include <lang/semantic/types/module.h>

// to throw errors
#include <common/diagnostic/diagnostic.h>

#include <lang/pipeline/syntax_driver.h>


namespace lang::pipeline
{
    void SyntaxDriver::set_import_paths(const std::vector<std::string>& _paths) {
        paths = &_paths;
    }

    void SyntaxDriver::process(const std::string& name, semantic::SemanticState* _state) {
        state = _state;
        process(name);
    }

    void SyntaxDriver::process(const std::string& module_name) {
        // check if module already processed
        if(state->program.modules.contains(module_name)) {
            throw common::diagnostic::InterError(std::format("module {} overloading", module_name));
        }
        // tring to fing module file
        // std::string file_name = module_name + ".lang";
        std::unique_ptr<common::streams::FileIStream> file = std::make_unique<common::streams::FileIStream>(module_name + ".lang");
        if(!file->is_open()) {
            if(!paths) {
                for(const auto& path_ : *paths) {
                    file->open(path_ + std::string(module_name + ".lang"));
                    if(file->is_open()) {
                        break;
                    }
                }
            }
        } if(!file->is_open()) throw common::diagnostic::InterError(std::format("Can not open file: {}.lang, or paths not given", module_name));

        // syntax
        syntax::lexer::Lexer lexer(file.get());
        auto tokens = lexer.tokenize();

        syntax::parser::Parser parser;
        auto syntax_container = parser.parse(tokens);

        // save data to semantic container

        /**
         * @todo save context data
         * 
         */
        auto module_ = semantic::Module::create(module_name
        ,                                       state->program.global_scope.get()
        ,                                       std::move(syntax_container.ast));
        // in table std::string_view, bsc name of module save's by module
        state->program.modules[module_->name] = std::move(module_);
    }
}
