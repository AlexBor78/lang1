#pragma once

#include <string>
#include <string_view>
#include <fstream>

#include <defs.h>

namespace lang
{
    class Stream
    {
    protected:
        Stream() = default;
    public:
        virtual std::string_view get_line() = 0;
        virtual std::string_view get_word() = 0;
        virtual bool is_eof() const noexcept = 0;
        virtual Position get_pos() const = 0;

        virtual char curr() const = 0;
        virtual char peak(size_t offset = 1) const = 0;
        virtual char next(size_t offset = 1) = 0;

        virtual void skip_whitespace() = 0;
    };

    class StringStream : public Stream
    {
    private:
        std::string str;
        size_t pos{0};
        bool is_end() const noexcept;
    public:
        virtual std::string_view get_line() override;
        virtual std::string_view get_word() override;
        virtual bool is_eof() const noexcept override;
        virtual Position get_pos() const override;

        virtual char curr() const override;
        virtual char peak(size_t offset = 1) const override;
        virtual char next(size_t offset = 1) override;
        virtual void skip_whitespace() override;

        StringStream() = delete;
        explicit StringStream(std::string_view _str):
            str(_str)
        {}
    };

    class FileStream : public Stream
    {
    private:
        std::string buf;
        size_t bufpos{0};

        std::string path;
        std::ifstream file;
        
        bool is_end() const noexcept;
    public:
        virtual std::string_view get_line() override;
        virtual std::string_view get_word() override;
        virtual bool is_eof() const noexcept override;
        virtual Position get_pos() const override;

        virtual char curr() const override;
        virtual char peak(size_t offset = 1) const override;
        virtual char next(size_t offset = 1) override;
        virtual void skip_whitespace() override;

        void openfile(std::string_view);

        FileStream() = default;
        explicit FileStream(std::string_view _path)
        {
            openfile(_path);
        }
    };
}
