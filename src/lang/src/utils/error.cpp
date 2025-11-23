#include <format>
#include <fstream>

#include <lang/utils/error.h>

namespace lang
{
    void CompileError::build_error() {
        std::string buf;
        if(pos.path.empty()) buf = std::format("error: {} \n", msg);
        else buf = std::format("error: {} in file {}\n", msg, pos.path); 

        std::fstream file(pos.path);
        if(!file) {
            if(!pos.path.empty()) buf += std::format("error opening file {}\n", pos.path);
            buf += std::format("error start in line {} in column {}\n", pos.start.line, pos.start.column);
            buf += std::format("error ends in line {} in column {}\n", pos.end.line, pos.end.column);
            buf.shrink_to_fit();
            msg = std::move(buf);
            return;
        }

        std::string source((std::istreambuf_iterator<char>(file)), {});
        if(pos.start.index >= source.size()) {
            buf += std::format("error: position {{line: {} column: {}}} is out of range file {}", pos.start.line, pos.start.column, pos.path);
            buf.shrink_to_fit();
            msg = std::move(buf);
            return;
        }

        // only for one-line tokens
        if(pos.start.line == pos.end.line) {
            size_t line_start = source.rfind("\n", pos.start.index);
            if(line_start == std::string::npos) line_start = 0;
            else ++line_start;

            size_t line_end = source.find("\n", pos.end.index);
            if(line_end == std::string::npos) line_end = source.size();

            std::string line = source.substr(line_start, line_end - line_start);

            buf += std::format("{}\n", line);

            size_t column = pos.end.column - pos.start.column;
            if(pos.length >= 3) buf += std::format("{}^{}^\n", std::string(column, ' '), std::string(pos.length - 2, '~'));
            if(pos.length == 2) buf += std::format("{}^^\n", std::string(column, ' '));
            if(pos.length == 1) buf += std::format("{}^\n", std::string(column, ' '));
            msg = std::move(buf);
        }
    }
}
