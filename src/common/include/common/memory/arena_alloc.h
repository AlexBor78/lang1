#pragma once

#include <memory>
#include <memory_resource>

#include <common/memory/alloc.h>

namespace common::memory
{

	class ArenaAlloc : public IAlloc {
	private:
		std::pmr::monotonic_buffer_resource arena;

	public:
		ArenaAlloc(const ArenaAlloc&) = delete;
    ArenaAlloc& operator=(const ArenaAlloc&) = delete;

		ArenaAlloc(
      size_t initial_size = INITIAL_SIZE,
      std::pmr::memory_resource* upstream = std::pmr::new_delete_resource()
    ):	arena(
      		initial_size
      	, upstream
      	)
    {}

		inline virtual std::pmr::memory_resource* get_resource() override {
			return &arena;
		}

		inline virtual void* allocate(size_t size, size_t align) override {
			return arena.allocate(size, align);
		}
		
		inline virtual void free() override {
			arena.release();
		}
	};
}
