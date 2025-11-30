#include <lang/semantic/semantic_types.h>

namespace lang::semantic
{

    SemanticState SemanticInitializer::init_state(std::string_view name) {
        SemanticState state;

        // program
        state.program.name = name;
        state.program.global_scope = init_global_scope();
        state.curr_scope = state.program.global_scope.get();

        return state;
    }

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
    std::unique_ptr<Scope> SemanticInitializer::init_global_scope() {
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
        scope->get_typetable().add_type("void", non_numeric);
        scope->get_typetable().add_type("bool", uint_info);

        // chars
        scope->get_typetable().add_type("char", uint_info);
        scope->get_typetable().add_type("char8", uint_info);
        scope->get_typetable().add_type("char16", uint_info);
        scope->get_typetable().add_type("char32", uint_info);
        scope->get_typetable().add_type("c_char", int_info);
        scope->get_typetable().add_type("c_uchar", uint_info);

        // int types
        scope->get_typetable().add_type("int", int_info);
        scope->get_typetable().add_type("int8", int_info);
        scope->get_typetable().add_type("int16", int_info);
        scope->get_typetable().add_type("int32", int_info);
        scope->get_typetable().add_type("int64", int_info);

        scope->get_typetable().add_type("uint", uint_info);
        scope->get_typetable().add_type("uint8", uint_info);
        scope->get_typetable().add_type("uint16", uint_info);
        scope->get_typetable().add_type("uint32", uint_info);
        scope->get_typetable().add_type("uint64", uint_info);

        // float types
        scope->get_typetable().add_type("half", float_info);
        scope->get_typetable().add_type("float", float_info);
        scope->get_typetable().add_type("double", float_info);

        scope->get_typetable().add_type("float16", float_info);
        scope->get_typetable().add_type("float32", float_info);
        scope->get_typetable().add_type("float64", float_info);
        scope->get_typetable().add_type("float128", float_info);

        return std::move(scope);
    }
}
