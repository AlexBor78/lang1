#include <lang/utils/logger.h>

namespace lang::utils
{
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

    Error Logger::stream_null() const {
        return Error("logger error: stream is null");
    }
    Error Logger::stream_bad() const {
        return Error("logger error: stream is bad");
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
    
    template<typename... Args>
    void Logger::debug(std::format_string<Args...> fmt, Args&&... args) noexcept {
        debug(std::format(fmt, std::forward<Args>(args)...));
    }
    void Logger::debug(const std::string& line) {
        if(!(level & Logger::LogLevel::DEBUG)) return;
        check_infostream();
        infostream->write_line("[DEBUG]" + line);
    }
    template<typename... Args>
    void Logger::log(std::format_string<Args...> fmt, Args&&... args) noexcept {
        log(std::format(fmt, std::forward<Args>(args)...));
    }
    void Logger::log(const std::string& line) {
        if(!(level & Logger::LogLevel::INFO)) return;
        check_infostream();
        infostream->write_line("[INFO]" + line);
    }
    template<typename... Args>
    void Logger::warn(std::format_string<Args...> fmt, Args&&... args) noexcept {
        warn(std::format(fmt, std::forward<Args>(args)...));
    }
    void Logger::warn(const std::string& line) {
        if(!(level & Logger::LogLevel::WARN)) return;
        if(check_errstream()) errstream->write_line("[WARN]" + line);
        else {  
            check_infostream();
            infostream->write_line("[WARN]" + line);  
        } 
    }
    template<typename... Args>
    void Logger::error(std::format_string<Args...> fmt, Args&&... args) noexcept {
        error(std::format(fmt, std::forward<Args>(args)...));
    }
    void Logger::error(const std::string& line) {
        if(!(level & Logger::LogLevel::ERROR)) return;
        if(check_errstream()) errstream->write_line("[WARN]" + line);
        else {  
            check_infostream();
            infostream->write_line("[ERROR]" + line);  
        } 
    }
}
