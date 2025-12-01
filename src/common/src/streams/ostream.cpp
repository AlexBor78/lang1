#include <common/streams/ostream.h>

namespace common::streams
{
// FileOStream

    void FileOStream::open(std::string_view _path) {
        path = _path;
        pos.path = path;
        if(!file) file = std::make_unique<std::ofstream>(path);
        else file->open(path);
        set_ostream(file.get());
    }
    
    bool FileOStream::is_open() const noexcept {
        return file && file->is_open();
    }

// StringOStream 

    std::string StringOStream::get_string() const noexcept {
        return str->str();
    }
}
