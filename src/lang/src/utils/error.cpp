#include <format>
#include <vector>
#include <lang/utils/istream.h>
#include <lang/utils/error.h>

namespace lang::errors
{
    void CompileError::build_error() {
        std::string buf;

        // check if pos is default (empty)
        if(pos == SourceLocation{}) {
            msg.shrink_to_fit();
            return;
        }

        if(pos.path.empty()) buf = std::format("error: {} \n", msg);
        else buf = std::format("error: {} in file {}\n", msg, pos.path); 

        utils::FileIStream file(pos.path);
        if(!file.is_open()) {
            if(!pos.path.empty()) buf += std::format("can not open file {}\n", pos.path);
            if(pos.start.line != pos.end.line) {
                buf += std::format("error start in line {} in column {}\n", pos.start.line, pos.start.column);
                buf += std::format("error ends in line {} in column {}\n", pos.end.line, pos.end.column);
            } else buf += std::format("error in line: {} columns: {}:{}\n", pos.start.line, pos.start.column, pos.end.column);
            msg = std::move(buf);
            msg.shrink_to_fit();
            return;
        }

        // check is there our token
        if(file.is_eof(pos.end.index)) {
            buf += std::format("error: position line: {} column: {} is out of range file {}", pos.end.line, pos.end.column, pos.path);
            buf.shrink_to_fit();
            msg = std::move(buf);
            return;
        }

        // convert file to std::vector<std::string> lines for convenience
        std::vector<std::string> lines;
        std::string buffer;
        while(!file.is_eof() && lines.size() <= pos.end.line + 1) {
            char c = file.peek();
            while(!file.is_eof() && c != '\n') {
                buffer += file.advance();
                if(!file.is_eof()) c = file.peek();
            } if(!file.is_eof() && c == '\n') buf += file.advance();
            
            buffer.shrink_to_fit();
            lines.emplace_back(std::move(buffer));
            buffer.clear();
        }

        std::string lines_buf;
        size_t offset;
        size_t num_width = std::to_string(pos.end.line).length();
            
        for (int i = 2; i >= 0; --i) { 
            if(pos.start.line < i) continue;
            size_t line_num = pos.start.line - i;
            if (line_num < lines.size()) {
                std::string padding(num_width - std::to_string(line_num).length(), ' ');
                lines_buf += std::format("{}{} | {}", padding, line_num, lines[line_num]);
            }
        } logger.debug("build_error(): check is lines processed correct\n{}", lines_buf);

        if(pos.length >= 3) buf += std::format("{}^{}^{}", std::string(pos.start.column, ' '), std::string(pos.length - 2, '~'), msg);
        if(pos.length == 2) buf += std::format("{}^^{}", std::string(pos.start.column, ' '), msg);
        if(pos.length == 1) buf += std::format("{}^{}", std::string(pos.start.column, ' '), msg);
        
        msg = std::move(buf);
        msg.shrink_to_fit();
    }
}
