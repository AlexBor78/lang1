#pragma once

#include <string>
#include <string_view>
#include <lang/common.h>
#include <lang/utils/diagnostic.h> // Diagnostic and DiagnosticBuilder

namespace lang::errors
{
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

    class CompileError : public Error
    {
    private:
        DiagnosticBuilder builder;

    protected:
        CompileError(std::string_view _msg
        ,            SourceLocation   _pos
        ,            std::string_view name = ""
        ):  Error(_msg)
        ,   builder(name, _pos) {
            msg = builder.build(_msg);
        }

    public: // api
        // virtual CompileError& operator=(const CompileError& e) noexcept {
        //     // todo
        //     return *this;
        // }
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

    class Warn : public Diagnostic
    {
    protected: // vars
        Warn(std::string_view _msg): 
            Diagnostic(_msg)
        {}

    public: // part of future api (will impl in inheritors)
        Warn(const Warn& e): 
            Warn(e.what())
        {}
    };

    class CompileWarn : public Warn
    {
    private:
        DiagnosticBuilder builder;

    protected:
        CompileWarn(std::string_view _msg
        ,            SourceLocation   _pos
        ,            std::string_view name = ""
        ):  Warn(_msg)
        ,   builder(name, _pos) {
            msg = builder.build(_msg);
        }

    public: // api
        virtual CompileWarn& operator=(const CompileWarn& e) noexcept {
            // todo
            return *this;
        }
    };

    class FrontendWarn : public CompileWarn
    {
    protected:
        FrontendWarn(std::string_view _msg
        ,             SourceLocation _pos = {}
        ,             std::string_view _name = "FrontendWarn"
        ):  CompileWarn(_msg
        ,                std::move(_pos)
        ,                _name
        ) {}
    };

    class LexerWarn : public FrontendWarn
    {
    public:
        explicit LexerWarn(std::string_view _msg
        ,                   SourceLocation _pos = {}
        ):  FrontendWarn(_msg
        ,                 std::move(_pos)
        ,                 "LexerWarn"
        ) {}
    };

    class ParserWarn : public FrontendWarn
    {
    public:
        explicit ParserWarn(std::string_view _msg
        ,                    SourceLocation   _pos = {}
        ):  FrontendWarn(_msg
        ,                 std::move(_pos)
        ,                 "ParserWarn"
        ) {}
    };
}
