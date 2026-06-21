#pragma once

#include <atomic>
#include <limits>
#include <cassert>
#include <stdexcept>

namespace common::utils {
	template<typename T> // c++20 generics 
	concept IDConcept = requires {
		typename T::IDType;
		requires std::constructible_from<T, typename T::IDType>;
	};

	template<IDConcept IDType>
	class IDGenerator {
		using CounterType = IDType::IDType;
		std::atomic<CounterType> next_id{0};

	public:
		IDGenerator() = default;
		IDType next() {
			CounterType curr = next_id.load();
			if(curr == std::numeric_limits<CounterType>::max())
				throw std::overflow_error("IDGenerator overflow");
			return IDType(next_id.fetch_add(1, std::memory_order_relaxed));
		}
	};
}
