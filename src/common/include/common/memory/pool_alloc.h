#pragma once

#include <memory_resource>
#include <common/memory/alloc.h>

namespace common::memory {
	class IPoolAlloc : public IAlloc {
		virtual void deallocate(void* ptr, size_t size, size_t align) = 0;
	};

	template<class PoolType>
	class TemplatePoolAlloc : public IPoolAlloc {
	private:
    static constexpr std::pmr::pool_options options{0, 1024 * 1024}; 
    PoolType pool;

	public:
    TemplatePoolAlloc(const TemplatePoolAlloc&) = delete;
    TemplatePoolAlloc& operator=(const TemplatePoolAlloc&) = delete;
		TemplatePoolAlloc(
      std::pmr::memory_resource* upstream = std::pmr::new_delete_resource()
		): pool(options, upstream)
		{}

		inline virtual std::pmr::memory_resource* get_resource() override {
      return &pool;
    }

		inline virtual void* allocate(size_t size, size_t align) override {
			return pool.allocate(size, align);
		}

		inline virtual void deallocate(void* ptr, size_t size, size_t align) override {
			pool.deallocate(ptr, size, align);
		}
		
		inline virtual void free() override {
			pool.release();
		}
	};

	using PoolAlloc = TemplatePoolAlloc<std::pmr::unsynchronized_pool_resource>;
	using AsyncPoolAlloc = TemplatePoolAlloc<std::pmr::synchronized_pool_resource>;
}
