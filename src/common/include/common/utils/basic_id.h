#pragma once

#include <cstdint>
#include <limits>
#include <cassert>
#include <functional>

namespace common::utils {
	template<size_t size>
	class BasicID {
	public:
		static_assert(
			size == 8 || size == 16 || size == 32 ||  size == 64
		,	"size must be 8, 16, 32, 64"
		);

		using IDType = std::conditional_t<
			(size == 8), 	uint8_t, 	std::conditional_t< // else
			(size == 16),	uint16_t, std::conditional_t< // else
			(size == 32),	uint32_t, // else
										uint64_t
		>>>;

		IDType id{std::numeric_limits<IDType>::max()};
		BasicID() = default;
		explicit BasicID(IDType _id): id(_id){}


		inline bool operator==(const BasicID<size> other) const noexcept {
			return id == other.id;
		}
		inline bool operator!=(const BasicID<size> other) const noexcept {
			return id != other.id;
		}
	};

	using ID8  = BasicID<8>;
	using ID16 = BasicID<16>;
	using ID32 = BasicID<32>;
	using ID64 = BasicID<64>;
}

namespace std {
	template<size_t size>
	struct hash<common::utils::BasicID<size>> {
		size_t operator()(const common::utils::BasicID<size>& id) const {
			return std::hash<typename common::utils::BasicID<size>::IDType>{}(id.id);
		}
	};
}
