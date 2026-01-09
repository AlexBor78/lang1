// #include <lang/semantic/analyzers/semantic_initializer.h>
// #include <memory>

namespace lang::semantic
{
    // void SemanticInitializer::init() {
    //     // initializing context
    //     semantic_state->context.scope_context.clear();
    //     semantic_state->context.types_context.clear();
    //     // extern list is already initialized by ModulesLoader

    //     // initializing program
    //     semantic_state->program.name = compile_options->output_name;
    //     semantic_state->program.global_scope = std::make_unique<Scope>();
    //     // modules(list) is already initialized by ModulesLoader

    //     semantic_state->enter_scope(semantic_state->program.global_scope.get());
    //     for(auto& it : semantic_state->program.modules) {
    //         init_module(*it.second);
    //     }

        
    // }

    // void SemanticInitializer::init_module(Module& module) {
    //     // if module already initialized
    //     if(module.interface_scope) return; 
    //     module.interface_scope = std::make_unique<Scope>(semantic_state->curr_scope);
    //     module.internal_scope = std::make_unique<Scope>(module.interface_scope.get());
        
    //     // initializing dependency modules
    //     for(auto depid : module.dependencies) init_module(*semantic_state->program.modules[depid]);
        
    //     // initializing submodules
    //     semantic_state->enter_scope(module.interface_scope.get());
    //     for(auto subid : module.submodules) init_module(*semantic_state->program.modules[subid]);
    //     semantic_state->leave_scope();
    // }

/*
Types table:

int     - int32
uint    - uint32
char    - int8
bool    - int1

half    - float16
float   - float32
double  - float64
*/
/*
    todo: move to types initializer
    void SemanticInitializer::init_global_scope() {
        auto scope = std::make_unique<Scope>();
        
        TypeInfo int_info = TypeInfo{
            .is_numeric = true,
            .is_integer = true,
            .is_signed = true
        };
        TypeInfo uint_info = TypeInfo{
            .is_numeric = true,
            .is_integer = true,
            .is_signed = false
        };
        TypeInfo float_info = TypeInfo{
            .is_numeric = true,
            .is_integer = false,
            .is_signed = true
        };
        TypeInfo non_numeric = TypeInfo{
            .is_numeric = false,
            .is_integer = false,
            .is_signed = false
        };

        // base types
        scope->get_typetable().add_builtin_type("void", non_numeric);
        scope->get_typetable().add_builtin_type("bool", uint_info);

        // chars
        scope->get_typetable().add_builtin_type("char", uint_info);
        scope->get_typetable().add_builtin_type("char8", uint_info);
        scope->get_typetable().add_builtin_type("char16", uint_info);
        scope->get_typetable().add_builtin_type("char32", uint_info);
        scope->get_typetable().add_builtin_type("c_char", int_info);
        scope->get_typetable().add_builtin_type("c_uchar", uint_info);

        // int types
        scope->get_typetable().add_builtin_type("int", int_info);
        scope->get_typetable().add_builtin_type("int8", int_info);
        scope->get_typetable().add_builtin_type("int16", int_info);
        scope->get_typetable().add_builtin_type("int32", int_info);
        scope->get_typetable().add_builtin_type("int64", int_info);

        scope->get_typetable().add_builtin_type("uint", uint_info);
        scope->get_typetable().add_builtin_type("uint8", uint_info);
        scope->get_typetable().add_builtin_type("uint16", uint_info);
        scope->get_typetable().add_builtin_type("uint32", uint_info);
        scope->get_typetable().add_builtin_type("uint64", uint_info);

        // float types
        scope->get_typetable().add_builtin_type("half", float_info);
        scope->get_typetable().add_builtin_type("float", float_info);
        scope->get_typetable().add_builtin_type("double", float_info);

        scope->get_typetable().add_builtin_type("float16", float_info);
        scope->get_typetable().add_builtin_type("float32", float_info);
        scope->get_typetable().add_builtin_type("float64", float_info);
        scope->get_typetable().add_builtin_type("float128", float_info);

        // return std::move(scope);
    }
*/
}
