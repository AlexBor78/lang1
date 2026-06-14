cmake_minimum_required(VERSION 3.10)
project(Lang)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CLANG_TIDY_EXE 
    NAMES 
        "clang-tidy" 
        "clang-tidy-16" "clang-tidy-17" "clang-tidy-18" "clang-tidy-19"
    HINTS 
        $ENV{CLANG_TIDY}  # Если твой скрипт задаст этот ENV, CMake возьмет его
)

if(CLANG_TIDY_EXE)
    message(STATUS ">>> Found clang-tidy: ${CLANG_TIDY_EXE}")
		#    set(CMAKE_CXX_CLANG_TIDY 
		#        "${CLANG_TIDY_EXE}"
		#        "-checks=-*,performance-*,bugprone-*,modernize-*"
		#        "--header-filter=^src/"
		#        "--system-headers=false"
		#    )
else()
    message(WARNING ">>> clang-tidy not found. Add 'export CLANG_TIDY=/путь/к/clang-tidy' в твой скрипт сборки.")
endif()

set(sources
    # common
    src/common/units_manager.cpp
		src/common/import_resolver.cpp

    # syntax
    src/syntax/lexer.cpp
    src/syntax/stmt.cpp
    src/syntax/expr.cpp
    src/syntax/ast.cpp
    src/syntax/parser.cpp

    # semantic types
    src/semantic/types/semantic_types.cpp
    src/semantic/types/typesystem.cpp
    src/semantic/types/identifier.cpp
    src/semantic/types/module.cpp
    src/semantic/types/scope.cpp
		src/semantic/types/hir.cpp

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
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
