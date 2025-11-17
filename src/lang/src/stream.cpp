
#include <cctype>

#include <stream.h>

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
    
    void StringStream::skip_whitespace() noexcept
    {
        while(pos < str.length() && isspace(str[pos]))
            ++pos;
    }
}
