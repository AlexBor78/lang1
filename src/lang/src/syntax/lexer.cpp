#include <common/common.h> // LEXER_DEBUG
#include <lang/utils/diagnostic.h>
#include <lang/syntax/lexer.h>
#include <lang/utils/syntax_utils.h>

namespace lang::syntax::lexer
{
// api
    bool Lexer::is_success() const noexcept {
        return success;
    }
    std::vector<Token> Lexer::tokenize(common::streams::InputStream* _stream) {
        stream = _stream;
        return tokenize();
    }
    std::vector<Token> Lexer::tokenize() {
        breakpoint();
        tokens.clear();

        if(stream->get_pos().path.empty()) logger.log("tokenizing new stream");
        else logger.log("tokenizing file: {}", stream->get_pos().path);
        
        while(!is_eof()) try {
            // for ConsoleIStream:
            // stop if got ctrl^D on POSIX or ctr+Z on shitdows
            if (peek() == '\x04' || peek() == '\x1A') {
                break;
            }
            if(isspace(peek())) {
                skip_whitespace();
                continue;
            }
            if(is_number()) {
                tokenize_number();
                continue;
            }
            if(is_string()) {
                tokenize_string();
                continue;
            }
            if(is_word()) {
                tokenize_word();
                continue;
            }
            if(is_comment()) {
                process_comment();
                continue;
            }
            
            tokenize_punct();
        } catch(const errors::LexerError& e) {
            success = false;
            logger.error("{}", e.what());
            if(!is_eof()) skip();
            else break;
        } catch(const std::exception& e) {
            success = false;
            logger.error("error while lexer working: {}", e.what());
            if(!is_eof()) skip();
            else break;
        }
        // tokens.emplace_back(Token{
        //     .ty = TokenType::END
        // });
        tokens.shrink_to_fit();
        return tokens;
    }

// inside api

    // errors creation

    void Lexer::init_logger() noexcept {
        logger.set_name("Lexer");
        #ifdef LEXER_DEBUG            
            logger.set_level(common::utils::Logger::LogLevel::ALL);
        #else
            logger.set_level(common::utils::Logger::LogLevel::INFO | common::utils::Logger::LogLevel::WARN | common::utils::Logger::LogLevel::ERROR);
        #endif 
    }

    void Lexer::breakpoint() noexcept {
        #ifdef LEXER_DEBUG
            debug_break();
        #endif
    }

    void Lexer::set_logger_infostream(std::unique_ptr<common::streams::OutputStream> stream) noexcept {
        logger.set_infostream(std::move(stream));
    }
    void Lexer::set_logger_errstream(std::unique_ptr<common::streams::OutputStream> stream) noexcept {
        logger.set_errstream(std::move(stream));
    }

    errors::LexerError Lexer::stream_null() const {
        return errors::LexerError("stream is null");
    }
    errors::LexerError Lexer::stream_bad() const {
        return errors::LexerError("stream is bad");
    }
    errors::LexerError Lexer::reached_eof() const {
        return errors::LexerError("reached eof");
    }
    errors::LexerError Lexer::passed_zero_to_eof() const {
        return errors::LexerError("passed zero to is_eof()");
    }
    errors::LexerError Lexer::word_start_num(common::SourceLocation pos) const {
        return errors::LexerError("word can not starts from number", pos);
    }
    errors::LexerError Lexer::not_closed_comment_block(common::SourceLocation pos) const {
        return errors::LexerError("\"/*\" comment block is not closed", pos);
    }
    errors::LexerError Lexer::not_closed_string(common::SourceLocation pos) const {
        return errors::LexerError("string block is not closed", pos);
    }
    errors::LexerError Lexer::wrong_number_format(common::SourceLocation pos) const {
        return errors::LexerError("wrong number format", pos);
    }
    errors::LexerError Lexer::unicode_not_suported(common::SourceLocation pos) const {
        return errors::LexerError("Unicode is not supported (yet)", pos);
    }

    // stream work

    void Lexer::check_stream() const {
        if(!stream) throw stream_null();
        if(stream->bad()) throw stream_bad();
    }
    void Lexer::check_data() const {
        if(is_eof()) throw reached_eof();
    }
    bool Lexer::is_eof(size_t n) const {
        check_stream();
        if(n == 0) throw passed_zero_to_eof();
        return stream->is_eof(n);
    }
    char Lexer::peek(size_t offset) const {
        check_data();
        return stream->peek(offset);
    }
    char Lexer::advance(size_t offset) {
        check_data();
        return stream->advance(offset);
    }
    void Lexer::skip(size_t n) {
        check_data();
        stream->skip(n);
    }
    std::string Lexer::read_word() {
        check_data();
        return stream->read_word();
    }
    void Lexer::skip_whitespace() {
        check_data();
        stream->skip_whitespace();
    }
    [[nodiscard("Lexer::update_pos() RETURN updated pos")]] common::SourceLocation Lexer::update_pos(common::SourceLocation pos, char c) noexcept {
        ++pos.length;
        ++pos.end.index;
        if(c == 'n') {
            ++pos.end.line;
            pos.end.column = 0;
        } else ++pos.end.column;
        return pos;
    }
    common::SourceLocation Lexer::get_pos() const {
        check_stream();
        return stream->get_pos();
    }

// tokenizing

    void Lexer::add_token(Token tok) {
        logger.debug("add_token() token: {{tt: {} str:\"{}\"}}", utils::stringify(tok.ty), tok.sym);
        tokens.emplace_back(tok);
    }

    bool Lexer::is_word() const noexcept {
        if(is_eof()) return false;
        return isalnum(peek()) || peek() == '_';
    }

    bool Lexer::is_number() const noexcept {
        if(is_eof()) return false;
        return isdigit(peek())
        ||     peek() == '.' && isdigit(peek(1));
    }

    bool Lexer::is_string() const noexcept {
        if(is_eof()) return false;
        return peek() == '"';
    }

    bool Lexer::is_comment() const noexcept {
        if(is_eof() || peek() != '/') return false;
        return peek(1) == '/'
        ||     peek(1) == '*';
    }

    void Lexer::tokenize_word() {
        breakpoint(); logger.debug("tokenize_word() keyword");
        common::SourceLocation pos = get_pos();
        std::string buf = read_word();
        
        pos.length = get_pos().start.index - pos.start.index;
        
        if(auto it = keywords.find(buf); it != keywords.end()) {
            add_token({
                .ty = it->second,
                .pos = pos,
                .sym = std::move(buf)
            }); return;
        }

        add_token({
            .ty = TokenType::IDENTIFIER,
            .pos = pos,
            .sym = std::move(buf)
        });
    }

    void Lexer::tokenize_punct() {
        breakpoint(); logger.debug("tokenize_punct()");
        common::SourceLocation pos = get_pos();
        std::string buf;

        for(int length = 3; length > 0; --length) {
            if(is_eof(length)) continue;

            pos.length = length;
            pos.end.line = pos.start.line;
            pos.end.index = pos.start.index + length;
            pos.end.column = pos.start.column + length;

            buf.clear();

            for(int i = 0; i < length; ++i)
                buf += peek(i);

            if(auto it = keywords.find(buf); it != keywords.end()) {
                skip(length);
                add_token({ 
                    .ty = it->second,
                    .pos = pos,
                    .sym = std::move(buf)
                }); return;
            }
        }
    }

    void Lexer::tokenize_number() {
        breakpoint(); logger.debug("tokenize_number()");
        common::SourceLocation pos = get_pos();
        std::string buf;
        bool has_dot{false};
        if(peek() == '.') {
            buf += '0';
        }

        while(!is_eof() && is_number()) {
            if(peek() == '.') {
                pos = update_pos(pos, peek());
                if(has_dot) throw wrong_number_format(pos);
                has_dot = true;
            } buf += advance();
        }

        add_token({
            .ty = TokenType::NUMBER,
            .pos = pos,
            .sym = std::move(buf)
        });
    }

    void Lexer::tokenize_string() {
        breakpoint(); logger.debug("tokenize_sring()");
        common::SourceLocation pos = get_pos();
        std::string buf;

        skip(); // skip '"'
        while(!is_eof() && peek() != '"') {
            pos = update_pos(pos, peek());
            if(peek() == '\\') {
                buf += tokenize_escape();
                continue;
            } buf += advance();
        }
        if(is_eof()) throw not_closed_string(pos);
        skip(); // skip '"'
        

        add_token({
            .ty = TokenType::STRING,
            .pos = pos,
            .sym = std::move(buf)
        });
    }

    char Lexer::tokenize_escape() {
        breakpoint();
        auto pos = get_pos();
        ++pos.end.column;
        ++pos.end.index;
        ++pos.end.line;
        ++pos.length;

        skip(); // skip '\'
        switch (peek()) {
            case ('\\'):    skip(); return '\\';
            case ('\"'):    skip(); return '\"';
            case ('n'):     skip(); return '\n';
            case ('t'):     skip(); return '\t';
            case ('u'):     throw unicode_not_suported(pos);
            case ('U'):     throw unicode_not_suported(pos);
            default:        return advance(); // skip char
        }
    }

    void Lexer::process_comment(){
        if(peek(0) != '/') return;
        if(peek(1) == '/') process_comment_line();
        if(peek(1) == '*') process_comment_block();
    }

    void Lexer::process_comment_line() {
        breakpoint(); logger.debug("process_coment_line()");
        skip(2); // skip "//"
        while(!is_eof() && peek() != '\n') skip();
    }
    void Lexer::process_comment_block() {
        breakpoint(); logger.debug("process_coment_block()");
        auto pos = get_pos();
        skip(2); // skip "/*"
        while(!is_eof()) {
            pos = update_pos(pos, peek());
            if(!is_eof(2)
            && peek(0) == '*'
            && peek(1) == '/') {
                skip(2); // sip "*/"
                return;
            } skip(); 
        } throw not_closed_comment_block(pos);
    }
}
