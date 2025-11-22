#pragma once

#include <string>
#include <exception>
#include <stdexcept>
#include <lang/common.h>


namespace lang
{
    class Error : public std::runtime_error
    {
    private:
        std::string msg;

    public:
        Error(const std::string& _msg)
        :  std::runtime_error(_msg)
        ,   msg(_msg)
        {}

        virtual const char* what() const noexcept override {
            return msg.c_str();   
        }

        Error& operator=(const std::exception& e) noexcept {
            msg = e.what();
            return *this;
        }
    };

    class CompileError : public Error
    {
    private:
        Position pos;
        std::string msg;

        void build_error();

    public:
        CompileError(const std::string& _msg, 
              Position _pos
        ):  Error(_msg)
        ,   msg(_msg)
        ,   pos(std::move(_pos))
        {build_error();}

        virtual const char* what() const noexcept override {
            return msg.c_str();   
        }

        CompileError& operator=(const CompileError& e) noexcept {
            pos = e.pos;
            msg = e.msg;
            return *this;
        }
        CompileError& operator=(const std::exception& e) noexcept {
            msg = e.what();
            return *this;
        }
    };
}
