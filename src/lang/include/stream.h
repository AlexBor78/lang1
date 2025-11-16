#pragma once

#include <string>
#include <string_view>

namespace lang
{
    class Stream
    {
    protected:
        Stream() = default;

    public:
        virtual std::string_view get_line() const noexcept = 0;
        virtual bool is_eof() const noexcept = 0;
    };

    class StringStream : public Stream
    {
    private:
        std::string str;
        bool is_end{false};
    public:
        virtual std::string_view get_line() const noexcept override;
        virtual bool is_eof() const noexcept override;

        StringStream() = delete;
        explicit StringStream(std::string_view _str):
            str(_str)
        {}
    };

} // namespace lang
