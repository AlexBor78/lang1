#pragma once

#include <string>
#include <exception>

namespace common::diagnostic
{
    class Diagnostic : public std::exception
    {
    protected:
        std::string msg;

        Diagnostic() = default;
        explicit Diagnostic(std::string_view _msg):
            msg(_msg)
        {}
    
    public:
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
    
    class Error: public Diagnostic
    {
    protected:
        Error() = default;
        explicit Error(std::string_view _msg):
            Diagnostic(_msg)
        {}
    };

    class InterError : public Error
    {
    public:
        explicit InterError(std::string_view _msg):
            Error(_msg)
        {}
    };

    class Warn : public Diagnostic
    {
    protected:
        Warn(std::string_view _msg): 
            Diagnostic(_msg)
        {}
    };   
}
