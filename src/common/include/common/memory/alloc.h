#pragma once

#include <memory>
#include <memory_resource>

namespace common::memory {

	constexpr int INITIAL_SIZE = 64 * 1024;

	/**
	 * @brief interface alloc
	 */
	class IAlloc {
	public:
		virtual ~IAlloc() = default;
		virtual std::pmr::memory_resource* get_resource() = 0;
		virtual void* allocate(size_t size, size_t align) = 0;
		virtual void free() = 0;
		
		template <typename Type, typename... Args>
    Type* make(Args&&... args) {
			void* ptr = allocate(sizeof(Type), alignof(Type));
			return std::construct_at(static_cast<Type*>(ptr), std::forward<Args>(args)...);
    }

	};
}
