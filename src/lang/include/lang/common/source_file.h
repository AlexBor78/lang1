#pragma once

#include <string>
#include <memory_resource>
#include <unordered_map>

#include <common/memory/arena_alloc.h>
#include <common/streams/istream.h>
#include <common/diagnostic/diagnostic.h>

// namespace lang::syntax {
// 	struct FileID {
// 		size_t id{ULLONG_MAX};
//     bool operator==(const FileID& o) const noexcept { return id == o.id; }
// 	};
// }
// 
// namespace std {
//      template<>
//      struct hash<lang::syntax::FileID> {
//          size_t operator()(const lang::syntax::FileID& id) const {
//              return hash<size_t>{}(id.id);
//          }
//      };
// }

namespace lang::syntax {
	class SourceFile {
	private:	
		std::unique_ptr<common::memory::ArenaAlloc> arena;
		std::string file_path;
		std::pmr::string file;

	public:
		SourceFile(
			std::string_view _file_path
		,	common::memory::ArenaAlloc* _arena
		):	arena(std::move(_arena))
		,		file_path(_file_path)
		,		file("", arena->get_resource())
		{}

//		inline FileID get_id() const noexcept {
//				return id;
//		}

		void load_from_stream(common::streams::InputStream* stream) {
			std::string raw = stream->read_all();
			file.assign(raw.data(), raw.size());
		}

		std::string_view content() const noexcept { return file; }
    std::string_view path() const noexcept { return file_path; }
	};


//	class SourcesStorage {
//	private:
//		size_t next_id{0};
//		std::unordered_map<std::string, FileID> paths;
//		std::unordered_map<FileID, std::unique_ptr<SourceFile>> storage;
//
//
//		constexpr inline static size_t calc_size_with_padding(
//				size_t file_size
//		,		size_t page_size = 4096
//		) noexcept {
//			size_t capacity = (file_size + page_size - 1) & ~(page_size - 1);
//			if(capacity == 0) return page_size;
//			return capacity;
//		}
//
//	public:
//		SourcesStorage() = default;
//
//		SourceFile* add(
//				const std::string& file_path
//		,		size_t file_size = 16 * 1024
//		,		size_t page_size = 4096
//		) {
//				if(paths.contains(file_path))
//					throw common::diagnostic::InterError("readding source to SourceStorage");
//
//				FileID id{.id = next_id++};
//
//				paths[file_path] = id;
//				storage[id] = std::make_unique<SourceFile>(
//					id
//				,	file_path
//				,	std::make_unique<common::memory::ArenaAlloc>(
//						calc_size_with_padding(
//							file_size
//						,	page_size
//						)
//					)
//				);
//				return storage.at(id).get();
//		}
//
//		SourceFile* get(FileID id) {
//			return storage.at(id).get();
//		}
//
//		SourceFile* get(const std::string& path) {
//			return storage.at(paths.at(path)).get();
//		}
//
//		inline bool contains(const std::string& path) const noexcept{
//			return paths.contains(path);
//		}
//		
//	};
}

