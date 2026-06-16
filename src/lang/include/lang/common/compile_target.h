#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include <common/memory/arena_alloc.h>
#include <common/memory/pool_alloc.h>
#include <common/utils/strings_storage.h>

#include <lang/common/source_file.h>
#include <lang/syntax/translation_unit.h>

namespace lang {
	struct TargetID{
		size_t id{ULLONG_MAX};
	};
}

namespace std {
	template<>
	struct hash<lang::TargetID> {
		size_t operator()(const lang::syntax::FileID& id) const {
			return hash<size_t>{}(id.id);
		}
	};
}

namespace lang {
	struct CompileTarget {
	private:
 		// do not use; it is upstream for 'pool' field
		std::unique_ptr<common::memory::PoolAlloc>	root_pool;																													 
	public:
		TargetID id;

		common::memory::PoolAlloc*  pool;

		common::memory::ArenaAlloc* source_arena{nullptr};
		syntax::SourceFile* source;

		common::memory::ArenaAlloc* ast_arena{nullptr};
		syntax::TranslationUnit* unit;

		common::memory::ArenaAlloc* hir_arena{nullptr};

		// for allocating scopes, symbols, etc
		common::memory::PoolAlloc* 	data_pool{nullptr};

		CompileTarget(
			TargetID _id
		,	std::unique_ptr<common::memory::PoolAlloc> _root_pool = nullptr
		):	root_pool(std::move(_root_pool))
		,		id(_id)
		{}
	};

	class TargetsStorage {
	private:
		size_t next_id{0};
		common::memory::AsyncPoolAlloc* pool;
		std::pmr::unordered_map<TargetID, CompileTarget*> storage;
		std::pmr::unordered_set<StringID> paths;

	public:
		TargetsStorage(
			common::memory::AsyncPoolAlloc* _pool
		):	pool(_pool)
		,		storage(pool->get_resource())
		,		paths(pool->get_resource())
		{}

		CompileTarget* add(
				std::unique_ptr<common::memory::PoolAlloc> _root_pool
		) {
			TargetID id{.id = next_id++};
			auto target = pool->make<CompileTarget>(
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
