#pragma once

#include <memory>
#include <memory_resource>

#include <common/memory/alloc.h>

namespace common::memory {

	class PoolAlloc : public IAlloc {
	private:
    static constexpr std::pmr::pool_options options{0, 1024 * 1024}; 
    std::pmr::unsynchronized_pool_resource pool;

	public:
    PoolAlloc(const PoolAlloc&) = delete;
    PoolAlloc& operator=(const PoolAlloc&) = delete;
		PoolAlloc(
      std::pmr::memory_resource* upstream = std::pmr::new_delete_resource()
		): pool(options, upstream)
		{}


		inline virtual std::pmr::memory_resource* get_resource() override {
        return &pool;
    }

		inline virtual void* allocate(size_t size, size_t align) override {
			return pool.allocate(size, align);
		}

		inline void deallocate(void* ptr, size_t size, size_t align) {
			pool.deallocate(ptr, size, align);
		}
		
		inline virtual void free() override {
			pool.release();
		}
	};
}
