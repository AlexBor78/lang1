#include <lang/pipeline/modules_loader.h>
#include <lang/pipeline/syntax_driver.h>

namespace lang::pipeline
{
    void ModulesLoader::load(semantic::SemanticState* _state) {
        state = _state;
        load();
    }

    void ModulesLoader::load() {
        if(paths) driver.set_import_paths(*paths);
        load("main");
    }

    void ModulesLoader::load(const std::vector<std::string>& modules) {
        for(const auto& module_ : modules) {
            load(module_);
            load(state->program.modules[module_]->dependencies);
        }
    }

    void ModulesLoader::load(const std::string& name) {
        if(!state->program.modules.contains(name)) {
            driver.process(name);
            state->program.modules[name]->dependencies = import_processor.process(state->program.modules[name]->ast);
            load(state->program.modules[name]->dependencies);
        }
    }
}
