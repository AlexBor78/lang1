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

    # pipeline
    src/pipeline/compile_driver.cpp
		src/pipeline/imports_resolver.cpp
		src/pipeline/modules_loader.cpp
    src/pipeline/syntax_driver.cpp
    src/pipeline/semantic_driver.cpp

    # syntax
    src/syntax/lexer.cpp
    src/syntax/ast.cpp
    src/syntax/parser.cpp

    # semantic types
		src/semantic/hir.cpp
    src/semantic/scope.cpp
    src/semantic/symbol.cpp
    src/semantic/typesystem.cpp

    # semantic analyzers
		# src/semantic/analyzers/semantic_initializer.cpp
    # src/semantic/analyzers/import_processor.cpp # removed
    # src/semantic/analyzers/symbols_collector.cpp # not in use yet
    
    # utils
    src/utils/diagnostic_builder.cpp
    src/utils/printast.cpp
    src/utils/utils.cpp
)

add_library(${PROJECT_NAME} STATIC ${sources})
target_link_libraries(${PROJECT_NAME} PUBLIC Common)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
