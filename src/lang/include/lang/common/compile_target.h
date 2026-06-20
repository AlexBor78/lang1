#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include <common/memory/arena_alloc.h>
#include <common/memory/pool_alloc.h>
#include <common/utils/strings_storage.h>

#include <lang/common/source_file.h>
#include <lang/syntax/translation_bundle.h>

namespace lang {
	struct TargetID{
		size_t id{ULLONG_MAX};

		inline bool operator==(const TargetID& other) const {
			return id == other.id;
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

namespace lang {
	struct CompileTarget {
	private:
 		// do not use; it is upstream for 'pool' field
		std::unique_ptr<common::memory::IPoolAlloc>	root_pool;																													 
	public:
		TargetID id;

		common::memory::PoolAlloc  pool;

		common::memory::ArenaAlloc* source_arena{nullptr};
		syntax::SourceFile* source;

		common::memory::ArenaAlloc* bundle_arena{nullptr};
		syntax::TranslationBundle* bundle;

		common::memory::ArenaAlloc* hir_arena{nullptr};
		common::memory::PoolAlloc* 	data_pool{nullptr};
		// CompileUnit unit; // it will be none semantic module (stores module symbol, hir, data structs, may be more)

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

		inline void create_bundle(syntax::AST ast) {
			bundle = bundle_arena->make<syntax::TranslationBundle>(std::move(ast));
		}
	};

	class TargetsStorage {
	private:
		size_t next_id{0};
		common::memory::IPoolAlloc* pool;
		std::pmr::unordered_map<TargetID, CompileTarget*> storage;
		std::pmr::unordered_set<StringID> paths;

	public:
		TargetsStorage(
			common::memory::IPoolAlloc* _pool
		):	pool(_pool)
		,		storage(pool->get_resource())
		,		paths(pool->get_resource())
		{}

		CompileTarget* add(
				std::unique_ptr<common::memory::IPoolAlloc> _root_pool
		) {
			TargetID id{.id = next_id++};
			auto target = _root_pool->make<CompileTarget>(
					id, std::move(_root_pool)
			);
			storage[id] = target;
			return target;
		}

		inline CompileTarget* get(TargetID id) {
			if(storage.contains(id)) return storage.at(id);
			return nullptr;
		}

		inline void add_path(StringID path) noexcept {
			paths.insert(path);
		}

		inline bool contains(StringID path) const noexcept {
			return paths.contains(path);
		}
	};
}
