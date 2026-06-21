#pragma once

#include <string>
#include <algorithm>
#include <string_view>
#include <unordered_map>

#include <common/utils/basic_id.h>
#include <common/utils/id_generator.h>
#include <common/memory/pool_alloc.h>
#include <common/diagnostic/diagnostic.h>

namespace lang {
	
	using StringID = common::utils::ID64;

	class StringsStorage {
	private:
		// AI MAGIC SHIT
		struct StringHash {
			using is_transparent = void;
			size_t operator()(std::string_view sv) const { return std::hash<std::string_view>{}(sv); }
			size_t operator()(const std::pmr::string& s) const { return std::hash<std::string_view>{}(s); }
		};

		struct StringEqual {
			using is_transparent = void;
			bool operator()(std::string_view lhs, std::string_view rhs) const { return lhs == rhs; }
			bool operator()(
				const std::pmr::string& lhs
			,	const std::pmr::string& rhs
			) const { return lhs == rhs; }
			bool operator()(std::string_view lhs, const std::pmr::string& rhs) const { return lhs == rhs; }
		};

		common::memory::IPoolAlloc* pool;
		common::utils::IDGenerator<StringID> gen;
		std::pmr::unordered_map<
			std::pmr::string
		, StringID
		, StringHash
		,	StringEqual
		> storage;

	public:
		StringsStorage(
			common::memory::IPoolAlloc* _pool
		): pool(_pool)
		{}

		StringID add(std::string_view sv) noexcept {
			if(auto it = storage.find(sv); it != storage.end()) return it->second;
			auto new_str = pool->make<std::pmr::string>(
				sv, pool->get_resource()
			);
			auto id = gen.next();
			storage[*new_str]	= id;
			return id;
		}

		inline bool contains(std::string_view sv) const noexcept {
			return storage.contains(sv);
		}

		std::string_view get(StringID id) const {
			auto it = std::ranges::find_if(storage, [id](const auto& pair) { 
				return pair.second == id; 
			});
			
			return it != storage.end()
				? std::string_view(it->first) 
				: throw common::diagnostic::InterError("StringID not found");
		}
	};
}
