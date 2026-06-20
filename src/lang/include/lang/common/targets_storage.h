#pragma once

#include <unordered_map>
#include <unordered_set>

#include <lang/common/compile_target.h>

namespace lang {
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
