cmake_minimum_required(VERSION 3.10)
project(Lang)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(sources
    src/stream.cpp
    src/tokenizer.cpp

    # ast
    src/ast/ast.cpp
    src/ast/stmt.cpp
    src/ast/expr.cpp

    # utils
    src/utils/utils.cpp
    src/utils/printast.cpp
)

add_library(${PROJECT_NAME} STATIC ${sources})

target_include_directories(${PROJECT_NAME} PUBLIC include)
