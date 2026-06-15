#pragma once

#include <cstdint>
#include <cassert>
#include <functional>

namespace common::utils {
	template<class Type>
	class BasicID {
	private:
		uintptr_t data;

		// constants
    static constexpr uintptr_t GLOBAL_FLAG = 1;
    static constexpr uintptr_t LOCAL_FLAG  = 0;

	public:
		template<class U> friend struct std::hash;
    BasicID() noexcept : data(0) {} 
		explicit BasicID(
			Type* ptr
		): data(reinterpret_cast<uintptr_t>(ptr) | LOCAL_FLAG)
		{assert((reinterpret_cast<uintptr_t>(ptr) & 1) == 0 && "Pointer must be aligned!");}
		
		explicit BasicID(
			int64_t id // NOT uint64_t - WE NEED SIGND
		): data((static_cast<uintptr_t>(id) << 1) | GLOBAL_FLAG)
		{assert(id >= 0 && "ID overflowed into sign bit!");}
		
		inline bool is_global() const noexcept {
			return (data & GLOBAL_FLAG) != 0;
		}
		
		inline int64_t get_global_id() const noexcept {
			assert(is_global() && "Trying to get global ID from a local pointer!");
			return data >> 1;
		}
		
		inline Type* get_local_ptr() noexcept {
			assert(!is_global() && "Trying to get local pointer from a global ID!");
			return reinterpret_cast<Type*>(data & ~GLOBAL_FLAG);
		}
		
		inline const Type* get_local_ptr() const noexcept {
			return const_cast<BasicID*>(this)->get_local_ptr();
		}
		
		inline bool operator==(const BasicID& other) const noexcept {
			return data == other.data;
		}
 
		inline bool operator!=(const BasicID& other) const noexcept {
			return data != other.data;
		}

	};
}

namespace std {
	template<class T>
	struct hash<common::utils::BasicID<T>> {
		inline size_t operator()(const common::utils::BasicID<T>& id) const {
			return hash<size_t>{}(id.data);
		}
	};
}
