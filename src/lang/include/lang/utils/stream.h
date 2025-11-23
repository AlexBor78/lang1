#pragma once

#include <format>
#include <string>
#include <ostream>
#include <istream>
#include <string_view>
#include <lang/common.h>
#include <lang/utils/error.h>

namespace lang::utils
{
    class AbstractStream
    {
    protected:
        Position pos;

    protected:
        AbstractStream() = default;
        Error stream_empty() const;
        Error stream_bad() const;
        Error reached_eof() const;
        Error passed_zero() const;
        
    public:
        virtual ~AbstractStream() = default;
        // check for n tokens, is there n tokens befor eof
        virtual bool is_eof(size_t n = 1) const = 0;
        virtual bool good() const noexcept = 0;
        virtual bool bad() const noexcept = 0;
        virtual Position get_pos() const = 0;
    };

    class InputStream : public AbstractStream
    {
    private:
        std::istream* istream{nullptr}; // not owned
        mutable bool is_eof_reached{false};
        mutable std::string lookahead_buffer;
        
        void check_stream() const;
        void update_pos(char) noexcept;
        void load_to_buf(size_t) const;

    protected:
        void set_istream(std::istream*);
        explicit InputStream(std::istream* _stream = nullptr):
            istream(_stream)
        {}

    public:
        virtual bool is_eof(size_t n = 1) const override;
        virtual bool good() const noexcept override;
        virtual bool bad() const noexcept override;
        virtual Position get_pos() const override;

        virtual std::string read_word();

        // if(offset == 0) return curr token
        // return curr+offset token
        virtual char peek(size_t offset = 0) const;

        // if(offset == 0) return curr token and move cursor 
        // return curr+offset token and move cursor
        virtual char advance(size_t offset = 0);

        virtual void skip(size_t n = 1);
        virtual void skip_whitespace();
    };

    class OutputStream : public AbstractStream
    {
    private:
        std::ostream* ostream{nullptr};

    protected:
        void set_ostream(std::ostream*);
        explicit OutputStream(std::ostream* _stream = nullptr):
            ostream(_stream)
        {}

    public:
        virtual bool is_eof(size_t n = 1) const override = 0;
        virtual Position get_pos() const override = 0;
        
        virtual void write_line(std::string_view) = 0;
        virtual void write_word(std::string_view) = 0;

        template<typename... Args>
        void write_format_line(std::format_string<Args...> fmt, Args&&...) noexcept;
        
        template<typename... Args>
        void write_format_word(std::format_string<Args...> fmt, Args&&...) noexcept;
        // todo: add cursor movements maybe
    };

    // useless i think
    // class IOStream : public InputStream, OutputStream
    // {
    // protected:
    //     IOStream() = default;
    // };
}
