#pragma once

namespace lang
{
    enum class TokeType
    {
        TODO,
        END
    };

    struct Position
    {
        const char* path{nullptr};
        int line{-1};
        int column{-1};
    };

    struct Token
    {
        TokeType ty{TokeType::END};
        Position pos;
        const char* sym{nullptr};
    };

} // namespace lang
