#pragma once

#include <string>
#include <fstream>
#include <string_view>
#include <lang/common.h>

namespace lang::utils
{
    class AbstractStream
    {
    public:
        virtual ~AbstractStream() = default;
        virtual bool is_eof() const noexcept = 0;
        virtual Position get_pos() const = 0;
    };

    class InputStream : public AbstractStream
    {
    protected:
        InputStream() = default;

    public:
        virtual bool is_eof() const noexcept override = 0;
        virtual Position get_pos() const override = 0;

        virtual std::string_view read_line() = 0;
        virtual std::string_view read_word() = 0;

        virtual char curr() const = 0;
        virtual char peak(size_t offset = 1) const = 0;
        virtual char next(size_t offset = 1) = 0;

        virtual void skip_whitespace() = 0;
    };

    class OutputStream : public AbstractStream
    {
    protected:
        OutputStream() = default;

    public:
        virtual bool is_eof() const noexcept override = 0;
        virtual Position get_pos() const override = 0;
        
        virtual void write_line(std::string_view) = 0;
        virtual void write_word(std::string_view) = 0;

        // virtual void write_format_line(std::string_view, args..) = 0;
        // virtual void write_format_word(std::string_view, args..) = 0;
        // todo: add cursor movements
    };

    class IOStream : public InputStream, OutputStream
    {
    protected:
        IOStream() = default;

    public:
        virtual bool is_eof() const noexcept override = 0;
        virtual Position get_pos() const override = 0;

        virtual std::string_view read_line() override = 0;
        virtual std::string_view read_word() override = 0;

        virtual void write_line(std::string_view) override = 0;
        virtual void write_word(std::string_view) override = 0;

        virtual char curr() const override = 0;
        virtual char peak(size_t offset = 1) const override = 0;
        virtual char next(size_t offset = 1) override = 0;

        virtual void skip_whitespace() override = 0;
    };

    class StringIStream : public InputStream
    {
    private:
        std::string str;
        size_t pos{0};
        bool is_end() const noexcept;

    public:
        virtual std::string_view read_line() override;
        virtual std::string_view read_word() override;
        virtual bool is_eof() const noexcept override;
        virtual Position get_pos() const override;

        virtual char curr() const override;
        virtual char peak(size_t offset = 1) const override;
        virtual char next(size_t offset = 1) override;
        virtual void skip_whitespace() override;

        StringIStream() = delete;
        explicit StringIStream(std::string_view _str):
            str(_str)
        {}
    };

    class FileIStream : public InputStream
    {
    private:
        std::string buf;
        size_t bufpos{0};

        std::string path;
        std::ifstream file;
        
        bool is_end() const noexcept;

    public:
        virtual std::string_view read_line() override;
        virtual std::string_view read_word() override;
        virtual bool is_eof() const noexcept override;
        virtual Position get_pos() const override;

        virtual char curr() const override;
        virtual char peak(size_t offset = 1) const override;
        virtual char next(size_t offset = 1) override;
        virtual void skip_whitespace() override;

        void open(std::string_view);

        FileIStream() = default;
        explicit FileIStream(std::string_view _path) {
            open(_path);
        }
    };
}
