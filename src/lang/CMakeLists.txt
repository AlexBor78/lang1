cmake_minimum_required(VERSION 3.10)
project(Lang)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(sources
    # common
    src/common/units_manager.cpp

    # syntax
    src/syntax/parser.cpp
    src/syntax/lexer.cpp

    # ast
    src/ast/stmt.cpp
    src/ast/expr.cpp
    src/ast/ast.cpp

    # semantic types
    src/semantic/types/semantic_types.cpp
    src/semantic/types/typesystem.cpp
    src/semantic/types/identifier.cpp
    src/semantic/types/module.cpp
    src/semantic/types/scope.cpp

    # semantic analyzers
    src/semantic/analyzers/semantic_initializer.cpp
    # src/semantic/analyzers/import_processor.cpp # removed
    # src/semantic/analyzers/symbols_collector.cpp # not in use yet

    # pipeline
    src/pipeline/compiledriver.cpp
    src/pipeline/syntax_driver.cpp
    src/pipeline/modules_loader.cpp
    src/pipeline/semantic_driver.cpp
    
    # utils
    src/utils/diagnostic_builder.cpp
    src/utils/printast.cpp
    src/utils/utils.cpp
    src/utils/error.cpp
)

add_library(${PROJECT_NAME} STATIC ${sources})
target_link_libraries(${PROJECT_NAME} PUBLIC Common)
target_include_directories(${PROJECT_NAME} PUBLIC include)
