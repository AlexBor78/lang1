#pragma once

#include <string>
#include <memory_resource>

#include <common/memory/arena_alloc.h>
#include <common/streams/istream.h>
#include <common/diagnostic/diagnostic.h>

namespace lang::syntax {
	class SourceFile {
	private:	
		common::memory::ArenaAlloc* arena;
		std::pmr::string file_path;
		std::pmr::string file;

	public:
		SourceFile(
			std::string_view _file_path
		,	common::memory::ArenaAlloc* _arena
		):	arena(_arena)
		,		file_path(_file_path, arena->get_resource())
		,		file("", arena->get_resource())
		{}

		void load_from_stream(common::streams::InputStream* stream) {
			std::string raw = stream->read_all();
			file.assign(raw.data(), raw.size());
		}

		std::string_view content() const noexcept { return file; }
    std::string_view path() const noexcept { return file_path; }
	};
}
