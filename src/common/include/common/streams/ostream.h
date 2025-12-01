#pragma once

#include <memory>
#include <sstream>
#include <fstream>
#include <iostream>
#include <common/streams/stream.h>

namespace common::streams
{
    class ConsoleOStream : public OutputStream
    {
    public:
        ConsoleOStream():
            OutputStream(&std::cout)
        {}
    };

    class ConsoleErrOStream : public OutputStream
    {
    public:
        ConsoleErrOStream():
            OutputStream(&std::cerr)
        {}
    };

    class FileOStream : public OutputStream
    {
    private:
        std::unique_ptr<std::ofstream> file{nullptr};
        std::string path;

    public:
        void open(std::string_view);
        bool is_open() const noexcept;

        explicit FileOStream(std::string_view _path):
            path()
        {open(_path);}
    };

    class StringOStream : public OutputStream
    {
    private:
        std::unique_ptr<std::ostringstream> str{nullptr};

    public:
        std::string get_string() const noexcept;

        StringOStream():
            str(std::make_unique<std::ostringstream>())
        {set_ostream(str.get());}
    };
}
