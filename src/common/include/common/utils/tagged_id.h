#pragma once

#include <limits>
#include <cstdint>
#include <cassert>
#include <functional>

namespace common::utils {
	template<class T>
	class TaggedID {
	public:
		using DataType	= uintptr_t;
		using IDType 		= intptr_t;
		using PtrType 	= T*;
		
	private:
    static constexpr DataType GLOBAL_FLAG = 1;
    static constexpr DataType LOCAL_FLAG  = 0;

		DataType data{std::numeric_limits<IDType>::max()};

	public:
		template<class U> friend struct std::hash;

    TaggedID() noexcept = default;
		
		explicit TaggedID(
			PtrType ptr
		): data(reinterpret_cast<DataType>(ptr) | LOCAL_FLAG)
		{assert((reinterpret_cast<DataType>(ptr) & 1) == 0 && "Pointer must be aligned!");}
		
		explicit TaggedID(
			IDType id
		): data((static_cast<DataType>(id) << 1) | GLOBAL_FLAG)
		{assert(id >= 0 && "ID overflowed into sign bit!");}
		
		inline bool is_global() const noexcept {
			return (data & GLOBAL_FLAG) != 0;
		}
		
		inline IDType get_id() const noexcept {
			assert(is_global() && "Trying to get global ID from a local pointer!");
			return static_cast<IDType>(data >> 1);
		}
		
		inline PtrType get_ptr() noexcept {
			assert(!is_global() && "Trying to get local pointer from a global ID!");
			return reinterpret_cast<PtrType>(data & ~GLOBAL_FLAG);
		}
		
		inline const PtrType get_local_ptr() const noexcept {
			return const_cast<TaggedID*>(this)->get_ptr();
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
	template<class T>
	struct hash<common::utils::TaggedID<T>> {
		inline size_t operator()(const common::utils::TaggedID<T>& id) const {
			return hash<size_t>{}(id.data);
		}
	};
}

