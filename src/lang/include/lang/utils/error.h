#pragma once

#include <string>
#include <exception>
#include <string_view>
#include <lang/common.h>
#include <lang/utils/logger.h>

namespace lang::errors
{
    class Error : public std::exception
    {
    protected:
        std::string msg;
        Error(std::string_view _msg): 
            msg(_msg)
        {}

    public:
        Error(const Error& e): 
            Error(e.what())
        {}
        virtual Error& operator=(const std::exception& e) noexcept {
            msg = e.what();
            return *this;
        }
        virtual Error& operator=(const Error& e) noexcept {
            msg = e.msg;
            return *this;
        }
    };

    class InterError : public Error
    {
    public:
        explicit InterError(std::string_view _msg):
            Error(_msg)
        {}
    };

    class CompileError : public Error
    {
    private:
        utils::Logger logger{utils::Logger::LogLevel::ALL};
        void init_logger(std::string_view name = "");
        SourceLocation pos;
        void build_error();

    protected:
        CompileError(std::string_view _msg
        ,            SourceLocation   _pos
        ,            std::string_view name = ""
        ):  Error(_msg)
        ,   pos(std::move(_pos)) {
            init_logger(name);
            build_error();
        }
    public:
        CompileError(const CompileError& e): Error(e.what()) {
            logger.set_name(e.get_name());
            logger.set_prefix(e.get_prefix());
            logger.set_level(e.logger.get_level());
        }
    public: // api
        virtual CompileError& operator=(const CompileError& e) noexcept {
            msg = e.msg;
            logger.set_name(e.get_name());
            logger.set_prefix(e.get_prefix());
            logger.set_level(e.logger.get_level());
            return *this;
        }
        std::string_view get_name() const noexcept {
            return logger.get_name();
        }
        std::string_view get_prefix() const noexcept {
            return logger.get_prefix();
        }
        virtual const char* what() const noexcept override {
            return msg.c_str();   
        }
    };

    class FrontendError : public CompileError
    {
    protected:
        FrontendError(std::string_view _msg
        ,             SourceLocation _pos = {}
        ,             std::string_view _name = "FrontendError"
        ):  CompileError(_msg
        ,                std::move(_pos)
        ,                _name
        ) {}
    };

    class LexerError : public FrontendError
    {
    public:
        virtual Error& operator=(const std::exception& e) noexcept override {
            msg = e.what();
            return *this;
        }
        explicit LexerError(std::string_view _msg
        ,                   SourceLocation _pos = {}
        ):  FrontendError(_msg
        ,                 std::move(_pos)
        ,                 "LexerError"
        ) {}
    };

    class ParserError : public FrontendError
    {
    public:
        virtual Error& operator=(const std::exception& e) noexcept override {
            msg = e.what();
            return *this;
        }
        explicit ParserError(std::string_view _msg
        ,                    SourceLocation   _pos = {}
        ):  FrontendError(_msg
        ,                 std::move(_pos)
        ,                 "ParserError"
        ) {}
    };
    
    // unused for now
    // class MiddlendError : public CompileError
    // {
    // protected:
    //     MiddlendError(std::string_view _msg
    //     ,             SourceLocation _pos = {}
    //     ,             std::string_view name = ""
    //     ):  CompileError(_msg
    //     ,                std::move(_pos)
    //     ,                "Middle"
    //     )
    //     {}
    // };
}
