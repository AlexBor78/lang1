#pragma once

#include <memory>
#include <memory_resource>

namespace common::memory
{
	constexpr int INITIAL_SIZE = 64 * 1024;

	class ArenaAloc {
	private:
		std::unique_ptr<std::byte[]> start_buf;
		std::pmr::monotonic_buffer_resource arena;

	public:
		ArenaAloc(
			size_t initial_size = INITIAL_SIZE
		): start_buf(std::make_unique<std::byte[]>(initial_size)) 
	 	,	 arena(
				start_buf.get()
			, initial_size
			, std::pmr::new_delete_resource()
			)
		{}
		
		ArenaAloc(const ArenaAloc&) = delete;
    ArenaAloc& operator=(const ArenaAloc&) = delete;

		template <typename Type, typename... Args>
    Type* alloc(Args&&... args) {
			void* ptr = arena.allocate(sizeof(Type), alignof(Type));
			return std::construct_at(static_cast<Type*>(ptr), std::forward<Args>(args)...);
    }

		inline std::pmr::memory_resource* get_resource() {
			return &arena;
		}

		inline void free() {
			arena.release();
		}
	};
}
