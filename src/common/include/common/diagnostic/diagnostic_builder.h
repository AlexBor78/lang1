#pragma once

#include <string>
#include <string_view>
#include <common/common.h>
#include <common/utils/logger.h>

namespace common::diagnostic
{
    class DiagnosticBuilder
    {
    private: // vars
        common::utils::Logger logger{common::utils::Logger::LogLevel::ALL};
        std::string builded_msg;
        bool is_builded{false}; 
        common::SourceLocation pos;
    
    private: // api
        void init_logger(std::string_view name = ""
        ,                common::utils::Logger::LogLevel level = common::utils::Logger::LogLevel::ALL);

    public: // api
        std::string build(std::string_view);
        explicit DiagnosticBuilder(std::string_view name = ""
        ,                          const common::SourceLocation& _pos = {}
        ,                          common::utils::Logger::LogLevel level = common::utils::Logger::LogLevel::ALL
        ): pos(_pos) {
            init_logger(name, level);
        }
        DiagnosticBuilder(const DiagnosticBuilder& other) {
            pos = other.pos;
            logger = other.logger;
            is_builded = other.is_builded;
            if(is_builded) builded_msg = other.builded_msg;
        }
    };
}
