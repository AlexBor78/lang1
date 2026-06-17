#pragma once

#include <cstddef>
#include <climits>

#include <memory>
#include <unordered_map>

#include <common/memory/arena_alloc.h>

#include <lang/syntax/ast/ast.h>
#include <lang/common/symbol_path.h>
#include <lang/common/source_file.h>

// namespace lang::syntax
// {
//     struct UnitID {
//         size_t id{ULLONG_MAX};
//         inline bool operator==(const UnitID& other) const noexcept {
//             return id == other.id;
//         }
//     };
// }
// 
// namespace std {
//     template<>
//     struct hash<lang::syntax::UnitID> {
//         size_t operator()(const lang::syntax::UnitID& id) const {
//             return hash<size_t>{}(id.id);
//         }
//     };
// }

namespace lang::syntax {
    struct TranslationUnit {
        syntax::AST ast;

				TranslationUnit(
					syntax::AST _ast
				):	ast(std::move(_ast))
				{}
    };

//    class UnitsStorage {
//    private:
//        size_t next_id{0};
//        std::unordered_map<UnitID, std::unique_ptr<TranslationUnit>> units;
//
//    public:
//
//        UnitID gen_new_id() {
//					UnitID id{.id = next_id++};
//					units[id] = std::make_unique<TranslationUnit>(id);
//					return id;
//				}
//
//				TranslationUnit* add() {
//						return units.at(gen_new_id()).get();
//				}
//
//				TranslationUnit* add(syntax::AST ast) {
//					UnitID id{.id = next_id++};
//					units[id] = std::make_unique<TranslationUnit>(
//							id
//					,		std::move(ast)
//					); return units.at(id).get();
//				}
//
// 				TranslationUnit* add(
// 						FileID fid
// 				,		size_t file_size
// 				,		syntax::AST ast
// 				) {
// 					UnitID id{.id = next_id++};
// 					units[id] = std::make_unique<TranslationUnit>(
// 							id
// 					,		std::move(ast)
// 					); return units.at(id).get();
// 				}
// 
//         inline TranslationUnit* get(UnitID id) noexcept {
// 						if(contains(id)) return units.at(id).get();
// 						return nullptr;
// 				}
// 
//         inline const TranslationUnit* get(UnitID id) const noexcept {
// 						if(contains(id)) return units.at(id).get();
// 						return nullptr;
// 				}
// 				
//         inline bool contains(UnitID id) const noexcept {
// 						return units.contains(id);
// 				}
//     };
}
