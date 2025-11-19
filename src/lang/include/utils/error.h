#pragma once

#include <defs.h>
#include <string>
#include <stdexcept>

namespace lang
{
    class CompileError : public std::runtime_error
    {
    private:
        Position pos;
        std::string msg;

        void build_error();

    public:
        CompileError(const std::string& _msg, 
              Position _pos
        ):  std::runtime_error(_msg)
        ,   msg(_msg)
        ,   pos(std::move(_pos))
        {build_error();}

        virtual const char* what() const noexcept override
        {
            return msg.c_str();   
        }
    };
}
