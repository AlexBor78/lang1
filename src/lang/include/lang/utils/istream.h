#pragma once

#include <memory>
#include <sstream>
#include <fstream>
#include <iostream>
#include <lang/utils/stream.h>

namespace lang::utils
{
    class FileIStream : public InputStream
    {
    private:
        std::unique_ptr<std::ifstream> file{nullptr};
        std::string path;

    public:
        void open(std::string_view);
        bool is_open() const noexcept;

        explicit FileIStream(std::string_view _path):
            path(_path)
        {open(_path);}
    };

    class StringIStream : public InputStream
    {
    private:
        std::unique_ptr<std::istringstream> str{nullptr};

    public:
        void set_string(const std::string&);
        std::string get_string() const noexcept;
        
        explicit StringIStream(const std::string&  _str) {
            set_string(_str);
        }
    };

    class ConsoleIStream : public InputStream
    {
    public:
        ConsoleIStream():
            InputStream(&std::cin)
        {}
    };
}
