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

        bool empty() const noexcept {
            return path.empty();
        }
    };

    /**
     * @brief complex path, with absolute and relative paths, and other info
     * @todo  cache hash to optimize
     */
    struct SymbolPath
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

        /**
         * @brief call normalize methods for paths
         */
        void normalize() {
            absolute_path.normalize();
            relative_path.normalize();
        }

        bool operator==(const SymbolPath& other) const noexcept {
            if(!is_relative && !other.is_relative) return absolute_path == other.absolute_path;
            if(absolute_path.empty() || other.absolute_path.empty()) {
                return false; // or better return relative_path == other.relative_path; ?
            } return absolute_path == other.absolute_path;
        }

        bool empty() const noexcept {
            if(is_relative) return relative_path.empty();
            return absolute_path.empty();
        }
    };
}

namespace std {
    /**
     * @todo optimize that shit (e.g. don't re-normalize str every time(use cache))
     */
    template<>
    struct hash<lang::SymbolPathRaw> {
        size_t operator()(const lang::SymbolPathRaw& path) const noexcept {
            std::string normalized_path = path.normalized_path;
            if(normalized_path.empty()) normalized_path = path.normalize();
            return std::hash<std::string>{}(normalized_path);
        }
    };

    /**
     * @todo optimize that shit (e.g. don't re-normalize str every time(use cache))
     */
    template<>
    struct hash<lang::SymbolPath> {
        size_t operator()(const lang::SymbolPath& path) const noexcept {
            if(!path.is_relative
            || !path.absolute_path.empty()) return std::hash<lang::SymbolPathRaw>{}(path.absolute_path);
            return std::hash<lang::SymbolPathRaw>{}(path.relative_path);
        }
    };
}
