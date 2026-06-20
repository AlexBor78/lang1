#pragma once

#include <memory>

#include <common/memory/pool_alloc.h>
#include <common/memory/arena_alloc.h>
#include <common/utils/strings_storage.h>

#include <lang/syntax/source_file.h>
#include <lang/syntax/translation_bundle.h>
#include <lang/semantic/semantic_unit.h>

namespace lang {
	struct TargetID {
		size_t id{ULLONG_MAX};

		inline bool operator==(const TargetID& other) const {
			return id == other.id;
		}
	};
	
	struct CompileTarget {
	private:
 		// do not use; it is upstream for 'pool' field
		std::unique_ptr<common::memory::IPoolAlloc>	root_pool;																													 
	public:
		TargetID id;

		common::memory::PoolAlloc  	pool;

		common::memory::ArenaAlloc* source_arena{nullptr};
		syntax::SourceFile* 				source;

		common::memory::ArenaAlloc* bundle_arena{nullptr};
		syntax::TranslationBundle* 	bundle;

		common::memory::ArenaAlloc* hir_arena{nullptr};
		common::memory::PoolAlloc* 	data_pool{nullptr};
		semantic::SemanticUnit 			unit;
		
	public:

		CompileTarget(
			TargetID _id
		,	std::unique_ptr<common::memory::IPoolAlloc> _root_pool = nullptr
		):	root_pool(std::move(_root_pool))
		,		id(_id)
		,		pool(root_pool->get_resource())
		{}

		inline void create_source_arena(size_t size) {
			source_arena = pool.make<common::memory::ArenaAlloc>(size);
		}

		inline syntax::SourceFile* create_source(std::string_view file_path) {
			return source = source_arena->make<syntax::SourceFile>(
				file_path
			,	source_arena
			);
		}

		inline void create_bundle_arena(size_t size) {
			bundle_arena = pool.make<common::memory::ArenaAlloc>(size);
		}

		inline void create_bundle(syntax::ast::AST ast) {
			bundle = bundle_arena->make<syntax::TranslationBundle>(std::move(ast));
		}
	};
}

namespace std {
	template<>
	struct hash<lang::TargetID> {
		size_t operator()(const lang::TargetID& id) const {
			return hash<size_t>{}(id.id);
		}
	};
}

