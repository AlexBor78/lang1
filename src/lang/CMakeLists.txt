cmake_minimum_required(VERSION 3.10)
project(Lang)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(sources
    # frontend
    src/frontend/parser.cpp
    src/frontend/lexer.cpp

    # ast
    src/ast/stmt.cpp
    src/ast/expr.cpp
    src/ast/ast.cpp

    # semantic
    src/semantic/semantic_initializer.cpp
    src/semantic/typesystem.cpp
    src/semantic/identifier.cpp
    src/semantic/module.cpp
    src/semantic/scope.cpp
    
    # utils
    src/utils/diagnostic.cpp
    src/utils/printast.cpp
    src/utils/istream.cpp
    src/utils/ostream.cpp
    src/utils/stream.cpp
    src/utils/logger.cpp
    src/utils/error.cpp
    src/utils/utils.cpp
)

add_library(${PROJECT_NAME} STATIC ${sources})
target_include_directories(${PROJECT_NAME} PUBLIC include)
