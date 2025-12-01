#pragma once

#include <string_view>
#include <common/common.h>
#include <common/diagnostic/diagnostic.h> // Diagnostic, Error, InterError, Warn
#include <common/diagnostic/diagnostic_builder.h> // DiagnosticBuilder

namespace lang::errors
{
    class CompileError : public common::diagnostic::Error
    {
    private:
        common::diagnostic::DiagnosticBuilder builder;

    protected:
        CompileError(std::string_view _msg
        ,            common::SourceLocation   _pos
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

    class SyntaxError : public CompileError
    {
    protected:
        SyntaxError(std::string_view _msg
        ,             common::SourceLocation _pos = {}
        ,             std::string_view _name = "FrontendError"
        ):  CompileError(_msg
        ,                std::move(_pos)
        ,                _name
        ) {}
    };

    class LexerError : public SyntaxError
    {
    public:
        explicit LexerError(std::string_view _msg
        ,                   common::SourceLocation _pos = {}
        ):  SyntaxError(_msg
        ,                 std::move(_pos)
        ,                 "LexerError"
        ) {}
    };

    class ParserError : public SyntaxError
    {
    public:
        explicit ParserError(std::string_view _msg
        ,                    common::SourceLocation   _pos = {}
        ):  SyntaxError(_msg
        ,                 std::move(_pos)
        ,                 "ParserError"
        ) {}
    };
    
    // unused for now
    // class SemanticError : public CompileError
    // {
    // protected:
    //     MiddlendError(std::string_view _msg
    //     ,             common::SourceLocation _pos = {}
    //     ,             std::string_view name = ""
    //     ):  CompileError(_msg
    //     ,                std::move(_pos)
    //     ,                "Middle"
    //     )
    //     {}
    // };

    class CompileWarn : public common::diagnostic::Warn
    {
    private:
        common::diagnostic::DiagnosticBuilder builder;

    protected:
        CompileWarn(std::string_view _msg
        ,            common::SourceLocation   _pos
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

    class SyntaxWarn : public CompileWarn
    {
    protected:
        SyntaxWarn(std::string_view _msg
        ,             common::SourceLocation _pos = {}
        ,             std::string_view _name = "FrontendWarn"
        ):  CompileWarn(_msg
        ,                std::move(_pos)
        ,                _name
        ) {}
    };

    class LexerWarn : public SyntaxWarn
    {
    public:
        explicit LexerWarn(std::string_view _msg
        ,                   common::SourceLocation _pos = {}
        ):  SyntaxWarn(_msg
        ,                 std::move(_pos)
        ,                 "LexerWarn"
        ) {}
    };

    class ParserWarn : public SyntaxWarn
    {
    public:
        explicit ParserWarn(std::string_view _msg
        ,                    common::SourceLocation   _pos = {}
        ):  SyntaxWarn(_msg
        ,                 std::move(_pos)
        ,                 "ParserWarn"
        ) {}
    };
}
