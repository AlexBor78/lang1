#include <common/diagnostic/diagnostic.h>
#include <common/streams/stream.h>

namespace common::streams
{
// AbstractStream

    diagnostic::InterError AbstractStream::stream_null() const {
        return diagnostic::InterError("stream error: stream is empty");
    }
    diagnostic::InterError AbstractStream::stream_bad() const {
        return diagnostic::InterError("stream error: stream is bad");
    }
    diagnostic::InterError AbstractStream::reached_eof() const {
        return diagnostic::InterError("stream error: reached eof");
    }
    diagnostic::InterError AbstractStream::passed_zero() const {
        return diagnostic::InterError("stream error: passed 0 to is_eof()");
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
        if(!is_eof_reached) return istream && istream->good();
        return !lookahead_buffer.empty();
    }

    bool InputStream::bad() const noexcept {
        return istream && istream->bad();
    }

    void InputStream::check_stream() const {
        if(!istream) throw stream_null();
        if(bad()) throw stream_bad();
    }

    void InputStream::update_pos(char c) noexcept {
        ++pos.start.index;
        if(c == '\n') {
            ++pos.start.line;
            pos.start.column = 0;
        } else ++pos.start.column;
    }

    void InputStream::load_to_buf(size_t offset) const {
        if(is_eof_reached) return;
        while(lookahead_buffer.length() <= offset) {
            auto c = istream->get();
            if(c == std::char_traits<char>::eof()) {
                is_eof_reached = true;
                return;
            } lookahead_buffer += static_cast<char>(c);
        }
    }

    SourceLocation InputStream::get_pos() const {
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
            c = peek()) { word += advance();
            if(is_eof()) break;
        } return std::move(word);
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
        ++pos.length;
        ++pos.end.index;
        if(c == 'n') {
            ++pos.end.line;
            pos.end.column = 0;
        } else ++pos.end.column;
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

    SourceLocation OutputStream::get_pos() const {
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
}
