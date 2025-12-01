#pragma once

#include <string>
#include <exception>
#include <string_view>
#include <lang/utils/logger.h>

namespace lang::errors
{
    class Diagnostic : public std::exception
    {
    protected:
        std::string msg;

        Diagnostic() = default;
        explicit Diagnostic(std::string_view _msg):
            msg(_msg)
        {}
    
    public: // part of future api (will impl in inheritors)
        virtual ~Diagnostic() = default;
        Diagnostic(const Diagnostic& other) {
            msg = other.msg;
        }
        Diagnostic(const std::exception& e) {
            msg = e.what();
        }
        virtual const char* what() const noexcept override {
            return msg.c_str();
        }
    };

    class DiagnosticBuilder
    {
    private: // vars
        utils::Logger logger{utils::Logger::LogLevel::ALL};
        std::string builded_msg;
        bool is_builded{false}; 
        SourceLocation pos;
    
    private: // api
        void init_logger(std::string_view name = ""
        ,                utils::Logger::LogLevel level = utils::Logger::LogLevel::ALL);

    public: // api
        std::string build(std::string_view);
        explicit DiagnosticBuilder(std::string_view name = ""
        ,                          const SourceLocation& _pos = {}
        ,                          utils::Logger::LogLevel level = utils::Logger::LogLevel::ALL
        ): pos(_pos) {
            init_logger(name, level);
        }
        DiagnosticBuilder(const DiagnosticBuilder& other) {
            pos = other.pos;
            logger = other.logger;
            is_builded = other.is_builded;
            if(is_builded) builded_msg = other.builded_msg;
        }
    };
}
