cmake_minimum_required(VERSION 3.10)
project(Lang)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(sources
    # syntax
    src/syntax/parser.cpp
    src/syntax/lexer.cpp

    # ast
    src/ast/stmt.cpp
    src/ast/expr.cpp
    src/ast/ast.cpp

    # semantic types
    src/semantic/types/semantic_initializer.cpp
    src/semantic/types/typesystem.cpp
    src/semantic/types/identifier.cpp
    src/semantic/types/module.cpp
    src/semantic/types/scope.cpp

    # semantic analyzers
    src/semantic/analyzers/import_processor.cpp
    src/semantic/analyzers/symbols_collector.cpp

    # pipeline
    src/pipeline/compiledriver.cpp
    src/pipeline/syntax_driver.cpp
    src/pipeline/modules_loader.cpp
    src/pipeline/semantic_driver.cpp
    
    # utils
    src/utils/printast.cpp
    src/utils/utils.cpp

    # common
    src/utils/error.cpp
)

add_library(${PROJECT_NAME} STATIC ${sources})
target_link_libraries(${PROJECT_NAME} PUBLIC Common)
target_include_directories(${PROJECT_NAME} PUBLIC include)
