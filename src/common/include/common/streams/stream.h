#pragma once

#include <format>
#include <string>
#include <ostream>
#include <istream>
#include <string_view>
#include <common/common.h>

namespace common::diagnostic {
    class InterError;
}
namespace common::streams
{
    class AbstractStream
    {
    protected:
        SourceLocation pos;

    protected:
        AbstractStream() = default;
        diagnostic::InterError stream_null() const;
        diagnostic::InterError stream_bad() const;
        diagnostic::InterError reached_eof() const;
        diagnostic::InterError passed_zero() const;
        
    public:
        virtual ~AbstractStream() = default;
        virtual bool good() const noexcept = 0;
        virtual bool bad() const noexcept = 0;
        virtual SourceLocation get_pos() const = 0;
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
        virtual bool is_eof(size_t n = 1) const;
        virtual bool good() const noexcept override;
        virtual bool bad() const noexcept override;
        virtual SourceLocation get_pos() const override;

        virtual std::string read_word();

        /**
         * @brief see current+offset token
         * 
         * @param offset 
         * @return current+offset char 
         */
        virtual char peek(size_t offset = 0) const;

        /**
         * @brief move cursor by offset and return last token
         * 
         * @param offset 
         * @return char 
         */
        virtual char advance(size_t offset = 0);

        virtual void skip(size_t n = 1);
        virtual void skip_whitespace();
    };

    class OutputStream : public AbstractStream
    {
    private:
        std::ostream* ostream{nullptr};

        void check_stream() const;
        void update_pos(char);
        void update_pos(std::string_view);

    protected:
        void set_ostream(std::ostream*);
        explicit OutputStream(std::ostream* _stream = nullptr):
            ostream(_stream)
        {}

    public:
        virtual bool good() const noexcept override;
        virtual bool bad() const noexcept override;
        virtual SourceLocation get_pos() const override;
        
        virtual void write_word(std::string_view);
        virtual void write_line(std::string_view);

        template<typename... Args>
        void write_line(std::format_string<Args...> fmt, Args&&... args) noexcept {
            write_line(std::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        void write_word(std::format_string<Args...> fmt, Args&&... args) noexcept {
            write_word(std::format(fmt, std::forward<Args>(args)...));
        }
        /**
         * @todo add cursor movements maybe
         */
    };

    // useless i think
    // class IOStream : public InputStream, OutputStream
    // {
    // protected:
    //     IOStream() = default;
    // };
}
