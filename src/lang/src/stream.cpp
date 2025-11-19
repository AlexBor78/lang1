
#include <cctype>

#include <stdexcept>
#include <stream.h>
#include <string_view>

namespace lang
{
// StringStream

    bool StringStream::is_end() const noexcept
    {
        return pos >= str.length();
    }

    std::string_view StringStream::get_line()
    {
        if(is_end()) return "";
        
        // look for '\n' from curr pos
        size_t start = pos;
        size_t newline_pos = str.find('\n', pos);

        if(newline_pos == std::string::npos) {
            pos = str.length();
            return std::string_view(str).substr(start);
        }
        
        pos = newline_pos + 1;
        return std::string_view(str).substr(start, newline_pos - start);
    }

    std::string_view StringStream::get_word()
    {
        if(is_end()) return "";

        size_t wordstart_pos = str.find_first_not_of(" \t\n", pos);
        if(wordstart_pos == std::string::npos) {
            pos = str.length();
            return "";
        }

        size_t wordend_pos = str.find_first_of(" \t\n", wordstart_pos);
        if(wordend_pos == std::string::npos) {
            pos = str.length();
            return std::string_view(str).substr(wordstart_pos);
        }

        pos = wordend_pos + 1;
        return std::string_view(str).substr(wordstart_pos, wordend_pos - wordstart_pos);
    }

    bool StringStream::is_eof() const noexcept
    {
        return is_end();
    }

    Position StringStream::get_pos() const
    {
        return {
            .path = "code string literal",
            .line = 0,
            .column = pos
        };
    }

    char StringStream::curr() const
    {        
        if(is_end()) return 0;
        return str[pos];
    }

    char StringStream::peak(size_t offset) const
    {
        if(is_end()) return 0;
        if(pos + offset < str.length())
            return str[pos + offset];
        return 0;
    }

    char StringStream::next(size_t offset)
    {
        pos += offset;
        if(is_end()) return 0;
        if(pos + offset < str.length())
            return str[pos];
        return 0;
    }
    
    void StringStream::skip_whitespace()
    {
        while(pos < str.length() && isspace(str[pos]))
            ++pos;
    }

// FileStream

    void FileStream::openfile(std::string_view _path)
    {
        file.close();
        buf.clear();
        path = _path;
        bufpos = 0;

        file.open(path);
        if(!file.is_open()) {
            throw std::runtime_error("couldn't open file" + path);
        }
        buf = {std::istreambuf_iterator<char>(file), {}};
    }

    bool FileStream::is_end() const noexcept
    {
        return bufpos >= buf.size();
    }

    std::string_view FileStream::get_line()
    {
        if(!file.is_open()) throw std::runtime_error("file isn't open");
        if(is_end()) return "";
        
        // look for '\n' from curr pos
        size_t start = bufpos;
        size_t newline_pos = buf.find('\n', bufpos);

        if(newline_pos == std::string::npos) {
            bufpos = buf.length();
            return std::string_view(buf).substr(start);
        }
        
        bufpos = newline_pos + 1;
        return std::string_view(buf).substr(start, newline_pos - start);
    }

    std::string_view FileStream::get_word()
    {
        if(!file.is_open()) throw std::runtime_error("file isn't open");
        if(is_end()) return "";

        size_t wordstart_pos = buf.find_first_not_of(" \t\n", bufpos);
        if(wordstart_pos == std::string::npos) {
            bufpos = buf.length();
            return "";
        }

        size_t wordend_pos = buf.find_first_of(" \t\n", wordstart_pos);
        if(wordend_pos == std::string::npos) {
            bufpos = buf.length();
            return std::string_view(buf).substr(wordstart_pos);
        }

        bufpos = wordend_pos + 1;
        return std::string_view(buf).substr(wordstart_pos, wordend_pos - wordstart_pos);
    }

    bool FileStream::is_eof() const noexcept
    {
        return is_end();
    }

    Position FileStream::get_pos() const
    {
        if(!file.is_open()) throw std::runtime_error("file isn't open");
        return {
            .path = path,
            .line = 0,
            .column = 0,
            .start = bufpos
        };
    }

    char FileStream::curr() const
    {        
        if(!file.is_open()) throw std::runtime_error("file isn't open");
        if(is_end()) return 0;
        return buf[bufpos];
    }

    char FileStream::peak(size_t offset) const
    {
        if(!file.is_open()) throw std::runtime_error("file isn't open");
        if(is_end()) return 0;
        if(bufpos + offset < buf.length())
            return buf[bufpos + offset];
        return 0;
    }

    char FileStream::next(size_t offset)
    {
        if(!file.is_open()) throw std::runtime_error("file isn't open");
        bufpos += offset;
        if(is_end()) return 0;
        if(bufpos + offset < buf.length())
            return buf[bufpos];
        return 0;
    }
    
    void FileStream::skip_whitespace()
    {
        if(!file.is_open()) throw std::runtime_error("file isn't open");
        while(bufpos < buf.length() && isspace(buf[bufpos]))
            ++bufpos;
    }

}
