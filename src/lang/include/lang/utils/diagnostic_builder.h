#pragma once

#include <string>
#include <string_view>
#include <common/common.h>
#include <common/utils/logger.h>
#include <common/streams/istream.h>

namespace lang::diagnostic
{
    /**
     * @brief generating diagnostic messages
     */
    class DiagnosticBuilder
    {
    private: // vars
        common::utils::Logger logger{common::utils::Logger::LogLevel::ALL};
        std::unique_ptr<common::streams::FileIStream> file;
        common::SourceLocation loc;
        std::string error_message;
        std::string builded_msg;
    
    private: // api
        void init_logger(std::string_view name = ""
        ,                common::utils::Logger::LogLevel level = common::utils::Logger::LogLevel::ALL
        );

        
        std::string read_line();

    public: // api
        std::string build();
        std::string build(std::string_view);

        explicit DiagnosticBuilder(std::string_view name = ""
        ,                          const common::SourceLocation& _pos = {}
        ,                          common::utils::Logger::LogLevel level = common::utils::Logger::LogLevel::ALL
        ): loc(_pos) {
            init_logger(name, level);
        }
        DiagnosticBuilder(const DiagnosticBuilder& other) {
            loc = other.loc;
            logger = other.logger;
            builded_msg = other.builded_msg;
        }
    };
}
