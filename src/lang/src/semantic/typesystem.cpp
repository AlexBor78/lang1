// #include <lang/utils/diagnostic.h>
// #include <lang/semantic/types/typesystem.h>
// 
// namespace lang
// {
// // TypeFlagsAPI
// 
//     WrapperType::WrapperKind WrapperType::get_kind() const noexcept {
//         return kind;
//     }
// 
// // TypeTable
// 
//     const CoreType* TypeTable::add_builtin_type(std::string_view name) {
//         if(contains(name)) common::diagnostic::InterError("Re-addition of type to typetable");
//         
//         auto type = pool->make<BuiltInType>(name);
//         core_table[type->get_name()] = std::move(type);
//         return  table[name];
//     }
// 
//     const CoreType* TypeTable::add_builtin_type(std::string_view name, TypeInfo info) {
//         if(contains(name)) throw common::diagnostic::InterError("Re-addition of type to typetable");
// 
//         auto type = pool->make<BuiltInType>(name, info);
//         core_table[type->get_name()] = std::move(type);
//         return  table[name];
//     }
// 
//     const CoreType* TypeTable::get_type(std::string_view name) const noexcept {
//         if(core_table.contains(name)) return core_table.at(name);
//         else return nullptr;
//     }
// 
//     bool TypeTable::contains(std::string_view name) const noexcept {
//         return core_table.contains(name);
//     }
// }
