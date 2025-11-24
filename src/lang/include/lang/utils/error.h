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
        utils::Logger logger{utils::Logger::LogLevel::ALL};
        std::string msg;
        Error(std::string_view _msg
        ,              std::string_view name = ""
        ): msg(_msg)
        {logger.set_name(name);}

    public:
        Error(const Error& e): Error(e.what()) {
            logger.set_name(e.get_name());
            logger.set_prefix(e.get_prefix());
            logger.set_level(e.logger.get_level());
        }
        virtual Error& operator=(const std::exception& e) noexcept {
            msg = e.what();
            return *this;
        }
        virtual Error& operator=(const Error& e) noexcept {
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

    class InterError : public Error
    {
    public:
        virtual Error& operator=(const std::exception& e) noexcept override {
            msg = e.what();
            return *this;
        }
        explicit InterError(std::string_view _msg
        ,                   std::string_view _name = ""
        ):  Error(_msg
        ,         _name
        ) {}
    };

    class CompileError : public Error
    {
    private:
        SourceLocation pos;
        void build_error();

    protected:
        CompileError(std::string_view _msg
        ,            SourceLocation   _pos
        ,            std::string_view name = ""
        ):  Error(_msg
            ,     name
            )
        ,   pos(std::move(_pos))
        {build_error();}
    };

    class FrontendError : public CompileError
    {
    protected:
        FrontendError(std::string_view _msg
        ,             SourceLocation _pos = {}
        ,             std::string_view name = ""
        ):  CompileError(_msg
        ,                std::move(_pos)
        ,                "Frontend"
        )
        {}
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
        ,                 "Lexer"
        )
        {}
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
        ,                 "Parser"
        )
        {}
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
