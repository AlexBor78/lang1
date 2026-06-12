#include <common/common.h> // LEXER_DEBUG
#include <lang/utils/diagnostic.h>
#include <lang/syntax/lexer.h>
#include <lang/utils/syntax_utils.h>

#include <print>

namespace lang::syntax::lexer
{
// public api
    bool Lexer::had_erros() const noexcept {
        return had_errors;
    }

    std::vector<Token> Lexer::tokenize() {
        breakpoint();
        tokens.clear();
				current_pos.path = path;

        while(!is_eof()) try {
//             // for ConsoleIStream:
//             // stop if got ctrl^D on POSIX or ctr+Z on shitdows
//             if (peek() == '\x04' || peek() == '\x1A') {
//                 break;
//             }
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
        } catch(const diagnostic::LexerError& e) {
            had_errors = true;
            logger->error("{}", e.what());
            if(!is_eof()) skip();
            else break;
        } catch(const std::exception& e) {
            had_errors = true;
            logger->error("lexer inter error: {}", e.what());
            if(!is_eof()) skip();
            else break;
        }
        tokens.shrink_to_fit();
        return tokens;
    }

// inside api

    // errors creation

    void Lexer::breakpoint() noexcept {
        #ifdef LEXER_DEBUG
            debug_break();
        #endif
    }

    diagnostic::LexerError Lexer::stream_null() const {
        return diagnostic::LexerError("stream is null");
    }
    diagnostic::LexerError Lexer::stream_bad() const {
        return diagnostic::LexerError("stream is bad");
    }
    diagnostic::LexerError Lexer::reached_eof() const {
        return diagnostic::LexerError("reached eof");
    }
    diagnostic::LexerError Lexer::passed_zero_to_eof() const {
        return diagnostic::LexerError("passed zero to is_eof()");
    }
    diagnostic::LexerError Lexer::unexpected_token(common::SourceLocation pos) const {
        return diagnostic::LexerError("unexpected token", pos);
    }
    diagnostic::LexerError Lexer::word_start_num(common::SourceLocation pos) const {
        return diagnostic::LexerError("word can not starts from number", pos);
    }
    diagnostic::LexerError Lexer::not_closed_comment_block(common::SourceLocation pos) const {
        return diagnostic::LexerError("\"/*\" comment block is not closed", pos);
    }
    diagnostic::LexerError Lexer::not_closed_string(common::SourceLocation pos) const {
        return diagnostic::LexerError("string block is not closed", pos);
    }
    diagnostic::LexerError Lexer::wrong_number_format(common::SourceLocation pos) const {
        return diagnostic::LexerError("wrong number format", pos);
    }
    diagnostic::LexerError Lexer::unicode_not_suported(common::SourceLocation pos) const {
        return diagnostic::LexerError("Unicode is not supported (yet)", pos);
    }

    // stream work
		// ai generated while refactoring; todo: refactor whole lexer,
		// so it operate only on characters (accepts string -> return tokens)
		void Lexer::check_stream() const {
    if (source.empty()) throw stream_null();
}

// check_data() больше не нужен в базовых методах, удали его или оставь только для специфичных проверок

bool Lexer::is_eof(size_t n) const {
    if (n == 0) throw passed_zero_to_eof();
    return cursor + n > source.size();
}

char Lexer::peek(size_t offset) const {
    check_stream(); // Было check_data(), стало check_stream() как в InputStream
    if (is_eof(offset + 1)) throw reached_eof();
    return source[cursor + offset];
}

char Lexer::advance(size_t offset) {
    check_stream(); // Было check_data(), стало check_stream()
    if (is_eof(offset + 1)) throw reached_eof();
    
    for (size_t i = 0; i <= offset; ++i) {
        current_pos = update_pos(current_pos, source[cursor + i]);
    }
    char result = source[cursor + offset];
    cursor += offset + 1;
    return result;
}

void Lexer::skip(size_t n) {
    if (n == 0) return;
    // Безопасный skip: останавливается на EOF, не кидает исключение
    size_t actual_skip = std::min(n, source.size() - cursor);
    for (size_t i = 0; i < actual_skip; ++i) {
        current_pos = update_pos(current_pos, source[cursor]);
        cursor++;
    }
}

std::string_view Lexer::mk_substr(size_t start, size_t lenth) {
	return source.substr(start, lenth);
}

std::string Lexer::read_word() {
    check_stream(); // Было check_data(), стало check_stream()
    std::string word;
    while (!is_eof() && is_word()) {
        word += advance();
    }
    return word;
}

void Lexer::skip_whitespace() {
    check_stream(); // Было check_data(), стало check_stream()
    while (!is_eof() && isspace(peek())) {
        skip(1);
    }
}

common::SourceLocation Lexer::get_pos() const {
    check_stream();
    return current_pos;
}

common::SourceLocation Lexer::get_loc() const {
		auto loc = get_pos();
		loc.path = path;
		return loc;
}

    [[nodiscard("Lexer::update_pos() RETURN updated pos")]]
		common::SourceLocation Lexer::update_pos(common::SourceLocation pos, char c) noexcept {
        ++pos.length;
        ++pos.end.index;
        if(c == '\n') {
            ++pos.end.line;
            pos.end.column = 0;
        } else ++pos.end.column;
        return pos;
    }

// tokenizing

    void Lexer::add_token(Token tok) {
        logger->debug(
						"add_token() token: {{tt: {} str:\"{}\"}}"
				, 	utils::stringify(tok.ty)
				, 	tok.sym
				);
        tokens.emplace_back(tok);
    }

    bool Lexer::is_word() const noexcept {
        if(is_eof()) return false;
        return isalnum(peek()) || peek() == '_';
    }

    bool Lexer::is_number() const noexcept {
        if(is_eof()) return false;
        return isdigit(peek())
        ||     (peek() == '.' && isdigit(peek(1)));
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
        breakpoint(); logger->debug("tokenize_word() called");

				auto start_loc = get_loc();
				size_t start = cursor;
				read_word();
				size_t end = cursor;
				auto word = mk_substr(start, end - start);
				auto end_loc = get_loc();

				start_loc.merge(end_loc);
				auto loc = start_loc;
        
        if(auto it = keywords.find(word); it != keywords.end()) {
            add_token({
                .ty = it->second,
                .pos = loc,
                .sym = word
                // .sym = word
            }); return;
        }

        add_token({
            .ty = TokenType::IDENTIFIER,
            .pos = loc,
            .sym = word
            // .sym = word
        });
    }

    void Lexer::tokenize_punct() {
        breakpoint(); logger->debug("tokenize_punct() called");
        common::SourceLocation start_loc = get_loc();
				size_t start = cursor;
        std::string_view punct;

        for(int length = 3; length > 0; --length) {
            if(is_eof(length)) continue;

						punct = mk_substr(start, length);
            if(auto it = keywords.find(punct); it != keywords.end()) {
                skip(length);
								start_loc.merge(get_loc());
                add_token({ 
                    .ty = it->second,
                    .pos = start_loc,
                    .sym = punct
                }); return;
            }
        } throw unexpected_token(start_loc);
    }
		
		// .34234 number format instead of 0.34234 unsupported for now
    void Lexer::tokenize_number() {
        breakpoint(); logger->debug("tokenize_number() called");
        common::SourceLocation loc = get_pos();
				size_t start = cursor, length = 0;
//				std::string buf; 
        bool has_dot{false};
//        if(peek() == '.') {
//					buf += "0";
//				}

        while(!is_eof() && is_number()) {
            if(peek() == '.') {
                loc = update_pos(loc, peek());

                if(has_dot) throw wrong_number_format(loc);
                has_dot = true;
            } ++length; skip();
        }

        add_token({
            .ty = TokenType::NUMBER,
            .pos = loc,
            .sym = mk_substr(start, length)
        });
    }

    void Lexer::tokenize_string() {
        breakpoint(); logger->debug("tokenize_sring() called");
        common::SourceLocation start_loc = get_loc();
				size_t start = cursor, length = 0;

        skip(); // skip '"'
        while(!is_eof() && peek() != '"') {
						++length; skip();
            if(peek() == '\\' && peek(1) == '"') {
								++length; skip(); continue;
            }
        }
        if(is_eof()) throw not_closed_string(start_loc);
        skip(); // skip '"'

        add_token({
            .ty = TokenType::STRING,
            .pos = start_loc,
            .sym = std::string(mk_substr(start, length))
        });
    }

//    char Lexer::tokenize_escape() {
//        breakpoint();
//        auto pos = get_pos();
//        ++pos.end.column;
//        ++pos.end.index;
//        ++pos.end.line;
//        ++pos.length;
//
//        skip(); // skip '\'
//        switch (peek()) {
//            case ('\\'):    skip(); return '\\';
//            case ('\"'):    skip(); return '\"';
//            case ('n'):     skip(); return '\n';
//            case ('t'):     skip(); return '\t';
//            case ('u'):     throw unicode_not_suported(pos);
//            case ('U'):     throw unicode_not_suported(pos);
//            default:        return advance(); // skip char
//        }
//    }

    void Lexer::process_comment(){
        if(peek(0) != '/') return;
        if(!is_eof(2) && peek(1) == '/') return process_comment_line();
        if(!is_eof(2) && peek(1) == '*') return process_comment_block();
    }

    void Lexer::process_comment_line() {
        breakpoint(); logger->debug("process_coment_line()");
        skip(2); // skip "//"

        while(!is_eof(1) && peek() != '\n') skip();
    }
    void Lexer::process_comment_block() {
        breakpoint(); logger->debug("process_coment_block()");
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
