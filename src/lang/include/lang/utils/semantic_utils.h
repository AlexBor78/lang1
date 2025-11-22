#pragma once

#include <lang/semantic/identifier.h>

namespace lang::utils
{
    constexpr const char* stringify(semantic::Identifier::Kind) noexcept;


    constexpr const char* stringify(semantic::Identifier::Kind kind) noexcept {
        switch (kind) {
            case(semantic::Identifier::Kind::NAMESPACE): return "NAMESPACE";
            case(semantic::Identifier::Kind::FUNCTOIN): return "FUNCTION";
            case(semantic::Identifier::Kind::VARIABLE): return "VARIABLE";
            case(semantic::Identifier::Kind::TYPE): return "TYPE";
        }
    }

}
