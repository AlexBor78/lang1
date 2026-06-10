#pragma once

#include <string>
#include <memory_resource>
#include <unordered_map>

#include <common/memory/arena_aloc.h>
#include <common/streams/istream.h>

namespace lang::syntax {
	class SourceFile {
	private:	
		std::pmr::string file;
		std::pmr::string file_path;

	public:
		SourceFile(
			std::string_view _file_path
		,	std::pmr::memory_resource* resource
		):	file("", resource)
		,		file_path(_file_path, resource)
		{}


		void load_from_stream(common::streams::InputStream* stream) {
			std::string raw = stream->read_all();
			file.assign(raw.data(), raw.size());
		}

		std::string_view content() const noexcept { return file; }
    std::string_view path() const noexcept { return file_path; }
	};


	class SourcesStorage {
	private:
		std::pmr::unordered_map<std::pmr::string, SourceFile*> storage;
	public:
		SourcesStorage(
			std::pmr::memory_resource* resource
		): storage(resource)
		{}


	};
}
