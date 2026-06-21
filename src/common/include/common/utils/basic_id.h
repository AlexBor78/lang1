#pragma once

#include <cstdint>
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
			(size == 8), 	uint8_t, 	std::conditional< // else
			(size == 16),	uint16_t, std::conditional< // else
			(size == 32),	uint32_t, // else
										uint64_t
		>>>;

		IDType id{std::numeric_limits<IDType>::max()};
		explicit BasicID(IDType _id): id(_id){}
	};

	using ID8  = BasicID<8>;
	using ID16 = BasicID<16>;
	using ID32 = BasicID<32>;
	using ID64 = BasicID<64>;

	template<class PtrType>
	class TaggedID {
	private:
		uintptr_t data;

		// constants
    static constexpr uintptr_t GLOBAL_FLAG = 1;
    static constexpr uintptr_t LOCAL_FLAG  = 0;

	public:
		template<class U> friend struct std::hash;
    TaggedID() noexcept : data(0) {} 
		explicit TaggedID(
			PtrType* ptr
		): data(reinterpret_cast<uintptr_t>(ptr) | LOCAL_FLAG)
		{assert((reinterpret_cast<uintptr_t>(ptr) & 1) == 0 && "Pointer must be aligned!");}
		
		explicit TaggedID(
			int64_t id // NOT uint64_t - WE NEED SIGNED, so we has 1 bit to store bool
		): data((static_cast<uintptr_t>(id) << 1) | GLOBAL_FLAG)
		{assert(id >= 0 && "ID overflowed into sign bit!");}
		
		inline bool is_global() const noexcept {
			return (data & GLOBAL_FLAG) != 0;
		}
		
		inline int64_t get_global_id() const noexcept {
			assert(is_global() && "Trying to get global ID from a local pointer!");
			return data >> 1;
		}
		
		inline PtrType* get_local_ptr() noexcept {
			assert(!is_global() && "Trying to get local pointer from a global ID!");
			return reinterpret_cast<PtrType*>(data & ~GLOBAL_FLAG);
		}
		
		inline const PtrType* get_local_ptr() const noexcept {
			return const_cast<TaggedID*>(this)->get_local_ptr();
		}
		
		inline bool operator==(const TaggedID& other) const noexcept {
			return data == other.data;
		}
 
		inline bool operator!=(const TaggedID& other) const noexcept {
			return data != other.data;
		}
	};
}

namespace std {
	template<size_t size>
	struct hash<common::utils::BasicID<size>> {
		size_t operator()(const common::utils::BasicID<size>& id) const {
			return std::hash<typename common::utils::BasicID<size>::IDType>{}(id.get());
		}
	};

	template<class T>
	struct hash<common::utils::TaggedID<T>> {
		inline size_t operator()(const common::utils::TaggedID<T>& id) const {
			return hash<size_t>{}(id.data);
		}
	};
}
