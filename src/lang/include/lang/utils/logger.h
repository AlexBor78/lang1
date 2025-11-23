#pragma once

#include <format>
#include <memory>
#include <lang/utils/error.h>
#include <lang/utils/ostream.h>

#define LOGLEVEL ::lang::utils::Logger::LogLevel::DEBUG

#ifdef LOGLEVEL

#define PARSER_LOGLEVEL LOGLEVEL

#endif

namespace lang::utils
{
    class Logger
    {
    public:
        enum class LogLevel : uint8_t
        {
            NONE  = 0,      // infostream
            DEBUG = 1 << 0, // infostream
            INFO  = 1 << 1, // infostream 
            WARN  = 1 << 2, // errsteam
            ERROR = 1 << 3  // errsteam
        };
    private:
        LogLevel level{LogLevel::WARN};
        std::unique_ptr<OutputStream> infostream{nullptr};
        std::unique_ptr<OutputStream> errstream{nullptr};

        Error stream_null() const;
        Error stream_bad() const;

        void check_infostream() const;
        bool check_errstream() const;

    public:
        LogLevel get_level() const  noexcept;
        void set_level(LogLevel) noexcept;
        void add_level(LogLevel) noexcept;
        void sub_level(LogLevel) noexcept;

        void set_infostream(std::unique_ptr<OutputStream>) noexcept;
        void set_errstream(std::unique_ptr<OutputStream>) noexcept;

        // print to console by default 
        Logger() = delete;
        Logger(LogLevel _level
        ,      std::unique_ptr<OutputStream> _infostream = std::make_unique<ConsoleOStream>()
        ,      std::unique_ptr<OutputStream> _errostream = std::make_unique<ConsoleOStream>()
        ):  level(_level)
        ,   infostream(std::move(_infostream))
        ,   errstream(std::move(_errostream))
        {}

        // debug lvl
        template<typename... Args>
        void debug(std::format_string<Args...> fmt, Args&&...) noexcept;
        void debug(const std::string&);

        // info lvl
        template<typename... Args>
        void log(std::format_string<Args...> fmt, Args&&...) noexcept;
        void log(const std::string&);

        // warn lvl
        template<typename... Args>
        void warn(std::format_string<Args...> fmt, Args&&...) noexcept;
        void warn(const std::string&);

        // error lvl
        template<typename... Args>
        void error(std::format_string<Args...> fmt, Args&&...) noexcept;
        void error(const std::string&);
    };

// LogLevel api

    constexpr Logger::LogLevel operator|(Logger::LogLevel a, Logger::LogLevel b) noexcept {
        return static_cast<Logger::LogLevel>(
            static_cast<uint8_t>(a) | static_cast<uint8_t>(b)
        );
    }

    constexpr Logger::LogLevel& operator|=(Logger::LogLevel& a, Logger::LogLevel b) noexcept {
        return a = static_cast<Logger::LogLevel>(
            static_cast<uint8_t>(a) | static_cast<uint8_t>(b)
        );
    }

    constexpr bool operator&(Logger::LogLevel a, Logger::LogLevel b) noexcept {
        return (static_cast<uint8_t>(a) & static_cast<uint8_t>(b)) != 0;
    }
}
