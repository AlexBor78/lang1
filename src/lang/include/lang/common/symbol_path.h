#pragma once

#include <string>
#include <vector>

namespace lang {

    /**
     * @brief just simple path, without any additional info
     * @todo use hash to optimize 
     */
    struct SymbolPathRaw {
        std::vector<std::string> path;
        std::string normalized_path;

        bool operator==(const SymbolPathRaw& other) const noexcept {
            if(normalized_path.empty() || other.normalized_path.empty()) return path == other.path;
            return normalized_path == other.normalized_path;
        }

        inline std::string normalize() const {
            std::string normalized;
            for(size_t i = 0; i < path.size(); ++i) {
                if(i) normalized += "::";
                normalized += path[i];
            } return normalized;
        }

        inline std::string normalize() {
            normalized_path = "";
            for(size_t i = 0; i < path.size(); ++i) {
                if(i) normalized_path += "::";
                normalized_path += path[i];
            } return normalized_path;
        }
    };
    /**
     * @brief temporay, to don't break old code
     */
    using SymbolPath = SymbolPathRaw;

    /**
     * @brief complex path, with absolute and relative paths, and other info
     * 
     */
    struct SymbolPath_
    {
        /**
         * @brief absolute path to symbol
         * 
         */
        SymbolPathRaw absolute_path;

        /**
         * @brief flag to easier code
         * 
         */
        bool is_relative{false};

        /**
         * @brief relative path to same symbol as absolute
         * @note without initialized absolute path can make problems
         */
        SymbolPathRaw relative_path;
    };
}

namespace std {
    /**
     * @todo optimize that shit (e.g. don't re-normalize str every time(use cache))
     */
    template<>
    struct hash<lang::SymbolPath> {
        size_t operator()(const lang::SymbolPath& path) const noexcept {
            std::string normalized_path = path.normalized_path;
            if(normalized_path.empty()) normalized_path = path.normalize();
            return std::hash<std::string>{}(normalized_path);
        }
    };
}
