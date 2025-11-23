#include "lang/common.h"
#include <lang/utils/stream.h>
#include <string_view>

namespace lang::utils
{
// AbstractStream

    Error AbstractStream::stream_null() const {
        return Error("stream error: stream is empty");
    }
    Error AbstractStream::stream_bad() const {
        return Error("stream error: stream is bad");
    }
    Error AbstractStream::reached_eof() const {
        return Error("stream error: reached eof");
    }
    Error AbstractStream::passed_zero() const {
        return Error("stream error: passed 0 to is_eof()");
    }
    
// InputStream

    void InputStream::set_istream(std::istream* _istream) {
        istream = _istream;
        pos = {};
        is_eof_reached = false;
    }

    bool InputStream::is_eof(size_t n) const {
        if (n == 0) throw passed_zero();
        if (lookahead_buffer.length() >= n) return false;
        load_to_buf(n);
        return lookahead_buffer.length() < n
        && is_eof_reached;
    }

    bool InputStream::good() const noexcept {
        return istream && istream->good();
    }

    bool InputStream::bad() const noexcept {
        return istream && istream->bad();
    }

    void InputStream::check_stream() const {
        if(!istream) throw stream_null();
        if(bad()) throw stream_bad();
    }

    void InputStream::update_pos(char c) noexcept {
        ++pos.start;
        if(c == '\n') {
            ++pos.line;
            pos.column = 0;
        } else ++pos.column;
    }

    void InputStream::load_to_buf(size_t offset) const {
        if(is_eof_reached) return;
        while(lookahead_buffer.length() <= offset) {
            auto c = istream->get();
            if(c == EOF) {
                is_eof_reached = true;
                return;
            }
            lookahead_buffer += static_cast<char>(c);
        }
    }

    Position InputStream::get_pos() const {
        check_stream();
        return pos;
    }

    char InputStream::peek(size_t offset) const {
        check_stream();
        if(is_eof(offset + 1)) throw reached_eof();
        return lookahead_buffer[offset];
    }

    char InputStream::advance(size_t offset) {
        check_stream();
        if(is_eof(offset + 1)) throw reached_eof();

        char c = lookahead_buffer[offset];

        for(size_t i = 0; i <= offset; ++i)
            update_pos(lookahead_buffer[i]);

        lookahead_buffer.erase(lookahead_buffer.begin(), lookahead_buffer.begin() + offset + 1);
        return c;
    }

    void InputStream::skip(size_t n) {
        if(n == 0) return;
        advance(n - 1);
    }

    std::string InputStream::read_word() {
        check_stream();
        std::string word;
        for(char c = peek(); 
            !is_eof() && (isalnum(c) || c == '_'); 
            c = peek()) word += advance();
        return std::move(word);
    }
    
    void InputStream::skip_whitespace() {
        check_stream();
        while(!is_eof() && isspace(peek())) skip();
    }

// OutputStream

    void OutputStream::check_stream() const {
        if(!ostream) throw stream_null();
        if(bad()) throw stream_bad();
    }

    void OutputStream::update_pos(char c) {
        ++pos.start;
        if(c == '\n') {
            ++pos.line;
            pos.column = 0;
        } else ++pos.column;
    }

    void OutputStream::update_pos(std::string_view str) {
        for(const auto& c :str) update_pos(c);
    }

    void OutputStream::set_ostream(std::ostream* _stream) {
        ostream = _stream;
    }

    bool OutputStream::good() const noexcept {
        return ostream && ostream->good();
    }

    bool OutputStream::bad() const noexcept {
        return ostream && ostream->bad();
    }

    Position OutputStream::get_pos() const {
        check_stream();
        return pos;
    }

    void OutputStream::write_word(std::string_view word) {
        check_stream();
        (*ostream) << word;
        update_pos(word);
    }
    void OutputStream::write_line(std::string_view line) {
        write_word(line);
        write_word("\n");
    }

    template<typename... Args>
    void OutputStream::write_format_line(std::format_string<Args...> fmt, Args&&... args) noexcept {
        write_line(std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void OutputStream::write_format_word(std::format_string<Args...> fmt, Args&&... args) noexcept {
        write_word(std::format(fmt, std::forward<Args>(args)...));
    }
}
