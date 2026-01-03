#pragma once

#include <string>
#include <vector>

namespace lang {

    /**
     * @todo use hash to optimize 
     */
    struct SymbolPath {
        std::vector<std::string> path;
        std::string normalized_path;

        bool operator==(const SymbolPath& other) const noexcept {
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
