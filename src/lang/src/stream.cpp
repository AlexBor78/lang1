#include <stream.h>
#include <string_view>

namespace lang
{
    std::string_view StringStream::get_line() const noexcept
    {
        if(!is_end)
        {
            return str;
        }
        return "";
    }

    bool StringStream::is_eof() const noexcept
    {
        return is_end;
    }

} // namespace lang
