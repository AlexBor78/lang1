#pragma once

#include <memory>
#include <string_view>
#include <lang/common.h>
#include <lang/utils/stream.h>

#define LOGLEVEL ::lang::utils::Logger::LogLevel::DEBUG

#ifdef LOGLEVEL 

#define PARSER_LOGLEVEL LOGLEVEL

#endif

namespace lang::utils
{
    class Logger
    {
    public:
        enum class LogLevel
        {
            DEBUG,
            INFO,
            WARN,
            ERROR,
            NONE
        };
    private:
        LogLevel level{};
        std::unique_ptr<OutputStream> stream;

    public:
        Logger() = delete;
        Logger(std::unique_ptr<OutputStream> _stream
        ,      LogLevel _level
        ):  stream(std::move(_stream))
        ,   level(_level)
        {}

        template<typename... Args>
        void log(std::format_string<Args...> fmt, Args&&...) noexcept;

        void log(std::string_view);
    };
}