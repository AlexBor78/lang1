#include <lang/utils/istream.h>

namespace lang::utils
{
// FileStream

    void FileIStream::open(std::string_view _path) {
        path = _path;
        pos.path = path;
        if(file) file->open(path);
        else file = std::make_unique<std::ifstream>(path);
        set_istream(file.get());
    }

    bool FileIStream::is_open() const noexcept {
        if(!file) return false;
        return file->is_open();
    }

// StringStream

    void StringIStream::set_string(const std::string& _str) {
        str = std::make_unique<std::istringstream>(_str);
        set_istream(str.get());
    }

    // creating new string
    std::string StringIStream::get_string() const noexcept {
        if(!str) return "";
        return str->str();
    }
}
