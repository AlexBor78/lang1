#include <format>
#include <fstream>

#include <utils/error.h>

namespace lang
{
    void CompileError::build_error()
    {
        std::string buf;
        buf = std::format("error: {}\nin file {}\n", msg, pos.path);

        std::fstream file(std::string(pos.path));
        if(!file) {
            buf += std::format("error opening file {}, ", pos.path);
            if((pos.line == 0 && pos.column == 0) && pos.start != 0) {
                buf += std::format("error in {} character good luck :)", pos.start);
                msg = std::move(buf);
            } else {
                buf += std::format("error in line {} in column {}", pos.line, pos.column);
                msg = std::move(buf);
            } return;
        }

        std::string source((std::istreambuf_iterator<char>(file)), {});
        if(pos.start >= source.size()) {
            buf += std::format("error: position {} out of range file {}", pos.start, pos.path);
            return;
        }

        size_t line_start = source.rfind("\n", pos.start);
        if(line_start == std::string::npos) line_start = 0;
        else ++line_start;

        size_t line_end = source.find("\n", pos.start);
        if(line_end == std::string::npos) line_end = source.size();

        std::string line = source.substr(line_start, line_end - line_start);

        buf += std::format("{}\n", line);

        size_t column = pos.start - line_start;
        if(pos.length > 3) buf += std::format("{}^{}^\n", std::string(column, ' '), std::string(pos.length - 2, '~'));
        if(pos.length == 2) buf += std::format("{}^^\n", std::string(column, ' '));
        if(pos.length == 1) buf += std::format("{}^\n", std::string(column, ' '));

        buf += std::format("debug: token length = {}", pos.length);

        msg = std::move(buf);
    }
}
