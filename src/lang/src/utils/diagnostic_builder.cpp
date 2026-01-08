#include <cassert>
#include <format>
// #include <vector>
#include <common/streams/istream.h>
#include <common/diagnostic/diagnostic.h>
#include <lang/utils/diagnostic_builder.h>

namespace lang::diagnostic
{
    void DiagnosticBuilder::init_logger(std::string_view name, common::utils::Logger::LogLevel) {
        // logger.set_level(common::utils::Logger::LogLevel::ALL);
        logger.set_level(common::utils::Logger::LogLevel::INFO | common::utils::Logger::LogLevel::WARN | common::utils::Logger::LogLevel::ERROR);
        logger.set_name(name);
    }

    
    std::string DiagnosticBuilder::build(std::string_view _error_message) {
        error_message = _error_message;
        return build();
    }
    

    std::string DiagnosticBuilder::build() { try {
        logger.debug("DiagnosticBuilder::build()");
        if(loc == common::SourceLocation()) {
            return error_message;
        }

        // open file to read
        file = std::make_unique<common::streams::FileIStream>(loc.path);
        if(!file->is_open()) throw common::diagnostic::InterError(std::format("can not open file {}", loc.path));
        for(int i = 0; i < loc.start.line; ++i) read_line();
        

        // plus 1 bcs of space
        int line_num_width = std::format("{}", loc.end.line + 1).length() + 1;
            builded_msg = "\n";
        if(loc.start.line == loc.end.line) {
            // adding line with error
            auto line = read_line();
            builded_msg += std::format("{} |{}", loc.start.line + 1, line);

            // adding arrow to show where is error
            assert(loc.end.column >= loc.start.column);
            int arrow_width = loc.end.column - loc.start.column;
            int arrow_start_offset = line_num_width + 1 + loc.start.column;
            std::string arrow(arrow_width, '^');

            // {number_line_offset}{" " instead of "|"}{offset_before_arrow}{arrow}{"\n"}
            builded_msg += std::format("{}{}\n", 
                std::string(arrow_start_offset, ' '),
                arrow
            );

            // adding error message
            builded_msg += std::format("{} {}",
                std::string(line_num_width, ' '),
                error_message
            ); builded_msg.shrink_to_fit();
        } else {
            for(int line = loc.start.line; line <= loc.end.line; ++line) builded_msg += std::format("{} |{}", line + 1, read_line());
            builded_msg += std::format("{} {}",
                std::string(line_num_width, ' '),
                error_message
            ); builded_msg.shrink_to_fit();
        } return builded_msg;
    } catch (const std::exception& e) {
        logger.error(e.what());
        builded_msg = std::format("Error while generating detailed error: {}\n", e.what());
        builded_msg += std::format("{}", error_message);
        return builded_msg;
    }}

    std::string DiagnosticBuilder::read_line() {
        std::string line;
        while(!file->is_eof() && file->peek() != '\n') line += file->advance();
        if(!file->is_eof()) line += file->advance();
        return std::move(line);
    }
}
