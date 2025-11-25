#include <lang/utils/error.h>
#include <lang/utils/logger.h>

namespace lang::utils
{
    void Logger::set_prefix(std::string_view _prefix) noexcept {
        prefix = _prefix;
    }
    std::string_view Logger::get_prefix() const noexcept {
        return prefix;
    }
    void Logger::remove_prefix() noexcept {
        prefix = "";
    }
    void Logger::set_name(std::string_view _name) noexcept {
        name = _name;
    }
    std::string_view Logger::get_name() const noexcept {
        return name;
    }
    void Logger::remove_name() noexcept {
        name = "";
    }

    Logger::LogLevel Logger::get_level() const  noexcept {
        return level;
    }
    void Logger::set_level(LogLevel lvl) noexcept {
        level = lvl;
    }
    void Logger::add_level(LogLevel lvl) noexcept {
        level |= lvl;
    }
    void Logger::sub_level(LogLevel lvl) noexcept {
        level = static_cast<LogLevel>(
            static_cast<uint8_t>(level) & ~static_cast<uint8_t>(lvl));
    }

    errors::InterError Logger::stream_null() const {
        return errors::InterError("logger error: stream is null");
    }
    errors::InterError Logger::stream_bad() const {
        return errors::InterError("logger error: stream is bad");
    }

    void Logger::set_infostream(std::unique_ptr<OutputStream> stream) noexcept {
        infostream = std::move(stream);
    }
    void Logger::set_errstream(std::unique_ptr<OutputStream> stream) noexcept {
        errstream = std::move(stream);
    }

    void Logger::check_infostream() const {
        if(!infostream) throw stream_null();
        if(infostream->bad()) throw stream_bad();
    }
    bool Logger::check_errstream() const {
        return  errstream && !errstream->bad();
    }
    
    void Logger::debug(const std::string& line) {
        if(!(level & Logger::LogLevel::DEBUG)) return;
        check_infostream();
        infostream->write_line("{}{}[{}{}DEBUG] {}", prefix, std::string(prefix.length() > 0,' '), name, std::string(name.length() > 0,' '), line);
    }
    void Logger::log(const std::string& line) {
        if(!(level & Logger::LogLevel::INFO)) return;
        check_infostream();
        infostream->write_line("{}{}[{}{}INFO] {}", prefix, std::string(prefix.length() > 0,' '), name, std::string(name.length() > 0,' '), line);
    }
    void Logger::warn(const std::string& line) {
        if(!(level & Logger::LogLevel::WARN)) return;
        if(check_errstream()) errstream->write_line("{}{}[{}{}WARN] {}", prefix, std::string(prefix.length() > 0,' '), name, std::string(name.length() > 0,' '), line);
        else {  
            check_infostream();
            infostream->write_line("{}{}[{}{}WARN] {}", prefix, std::string(prefix.length() > 0,' '), name, std::string(name.length() > 0,' '), line);
        } 
    }
    void Logger::error(const std::string& line) {
        if(!(level & Logger::LogLevel::ERROR)) return;
        if(check_errstream()) errstream->write_line("{}{}[{}{}ERROR] {}", prefix, std::string(prefix.length() > 0,' '), name, std::string(name.length() > 0,' '), line);
        else {  
            check_infostream();
            infostream->write_line("{}{}[{}{}ERROR] {}", prefix, std::string(prefix.length() > 0,' '), name, std::string(name.length() > 0,' '), line);
        } 
    }
}
