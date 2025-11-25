#include <format>
#include <vector>
#include <lang/utils/istream.h>
#include <lang/utils/error.h>

namespace lang::errors
{
    void CompileError::init_logger(std::string_view name) {
        // logger.set_level(utils::Logger::LogLevel::ALL);
        logger.set_level(utils::Logger::LogLevel::INFO | utils::Logger::LogLevel::WARN | utils::Logger::LogLevel::ERROR);
        logger.set_name(name);
    }
    void CompileError::build_error() { try {
        logger.debug("build_error() started");
        std::string buf;

        // check if pos is default (empty)
        if(pos == SourceLocation{}) {
            msg.shrink_to_fit();
            return;
        }
        if(pos.path.empty()) buf = std::format("{} \n", msg);
        else buf = std::format("in file {} {}\n", pos.path, msg); 

        utils::FileIStream file(pos.path);
        if(!file.is_open()) {
            if(!pos.path.empty()) {
                logger.log("cant open file {}", pos.path);
                buf += std::format("can not open file {}\n", pos.path);
            }
            if(pos.start.line != pos.end.line) {
                buf += std::format("error start in line {} in column {}\n", pos.start.line, pos.start.column);
                buf += std::format("error ends in line {} in column {}\n", pos.end.line, pos.end.column);
            } else buf += std::format("error in line: {} columns: {}:{}\n", pos.start.line, pos.start.column, pos.end.column);
            msg = std::move(buf);
            msg.shrink_to_fit();
            return;
        }

        // check is there our token
        if(file.is_eof(pos.end.index != 0 && pos.end.index)) {
            logger.log("index is out of range in  file {}", pos.path);
            buf += std::format("error: position line: {} column: {} is out of range file {}", pos.end.line, pos.end.column, pos.path);
            msg = std::move(buf);
            msg.shrink_to_fit();
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
            } if(!file.is_eof() && c == '\n') buffer += file.advance();
            
            buffer.shrink_to_fit();
            lines.emplace_back(std::move(buffer));
            buffer.clear();
        }

        // save formatted lines to lines_buf
        std::string lines_buf;
        // context size in lines
        const size_t context_size = 3;
        size_t num_width = std::to_string(pos.end.line).length();
            
        for (int i = context_size - 1; i >= 0; --i) { 
            if(pos.start.line < i) continue;
            size_t line_num = pos.start.line - i;
            if (line_num < lines.size()) {
                std::string padding(num_width - std::to_string(line_num).length(), ' ');
                lines_buf += std::format("{}{} | {}", padding, line_num + 1, lines[line_num]);
            }
        } logger.debug("build_error(): check is lines processed correct\n{}", lines_buf);

        buf += lines_buf;

        if(pos.start.line == pos.end.line) {
            if(pos.length >= 3) buf += std::format("{}^{}^{}", std::string(num_width + pos.start.column, ' '), std::string(pos.length - 2, '~'), msg);
            if(pos.length == 2) buf += std::format("{}^^{}", std::string(num_width + pos.start.column, ' '), msg);
            if(pos.length == 1) buf += std::format("{}^{}", std::string(num_width + pos.start.column, ' '), msg);
        } else buf += std::format("{} |{}^ {}", std::string(num_width, ' '), std::string(num_width + pos.start.column, ' '), msg);

        logger.debug("build_error(): builded msg: {}", buf);
        
        msg = std::move(buf);
        msg.shrink_to_fit();
    } catch(const std::exception& e) {
        logger.error("error while generating error message: {}", e.what());        
        if(pos != SourceLocation{}) {
            msg = std::format("in file {} at {}:{} {} (failed to generate detailed context)", pos.path, pos.start.line, pos.start.column, msg);
        } else msg += " (failed to generate detailed context, and position is empty)";
    } catch (...) {
        logger.error("Unknown error while generating error message");
        msg = "Internal compiler error: failed to generate error message";
    }}
}
