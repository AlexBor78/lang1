#pragma once
#include <format>
#include <memory>
#include <string>
#include <string_view>
#include <lang/utils/ostream.h>

#define LOGLEVEL ::lang::utils::Logger::LogLevel::DEBUG

#ifdef LOGLEVEL

#define PARSER_LOGLEVEL LOGLEVEL

#endif

namespace lang::errors {
    class InterError;
}
namespace lang::utils
{
    class ConsoleOStream;
    class Logger
    {
    public:
        enum class LogLevel : uint8_t
        {
            NONE  = 0,      // infostream
            DEBUG = 1 << 0, // infostream
            INFO  = 1 << 1, // infostream 
            WARN  = 1 << 2, // errsteam
            ERROR = 1 << 3,  // errsteam
            ALL   = DEBUG | INFO | WARN | ERROR
        };
    private:
        LogLevel level{LogLevel::WARN};
        std::string prefix;
        std::string name;

        std::shared_ptr<OutputStream> infostream{nullptr};
        std::shared_ptr<OutputStream> errstream{nullptr};

    private:
        errors::InterError stream_null() const;
        errors::InterError stream_bad() const;

        void check_infostream() const;
        bool check_errstream() const;

    public:
        void set_prefix(std::string_view) noexcept;
        std::string_view get_prefix() const noexcept;
        void remove_prefix() noexcept;

        void set_name(std::string_view) noexcept;
        std::string_view get_name() const noexcept;
        void remove_name() noexcept;

        LogLevel get_level() const  noexcept;
        void set_level(LogLevel) noexcept;
        void add_level(LogLevel) noexcept;
        void sub_level(LogLevel) noexcept;

        void set_infostream(std::unique_ptr<OutputStream>) noexcept;
        void set_errstream(std::unique_ptr<OutputStream>) noexcept;

        // print to console by default 
        Logger() = delete;
        Logger(LogLevel _level
        ,      std::shared_ptr<OutputStream> _infostream = std::make_shared<ConsoleOStream>()
        ,      std::shared_ptr<OutputStream> _errostream = std::make_shared<ConsoleErrOStream>()
        ):  level(_level)
        ,   infostream(std::move(_infostream))
        ,   errstream(std::move(_errostream))
        {}

        // debug lvl
        void debug(const std::string&);
        template<typename... Args>
        void debug(std::format_string<Args...> fmt, Args&&... args) noexcept {
            debug(std::format(fmt, std::forward<Args>(args)...));
        }

        // info lvl
        void log(const std::string&);
        template<typename... Args>
        void log(std::format_string<Args...> fmt, Args&&... args) noexcept {
            log(std::format(fmt, std::forward<Args>(args)...));
        }

        // warn lvl
        void warn(const std::string&);
        template<typename... Args>
        void warn(std::format_string<Args...> fmt, Args&&... args) noexcept {
            warn(std::format(fmt, std::forward<Args>(args)...));
        }

        // error lvl
        void error(const std::string&);
        template<typename... Args>
        void error(std::format_string<Args...> fmt, Args&&... args) noexcept {
            error(std::format(fmt, std::forward<Args>(args)...));
        }
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
