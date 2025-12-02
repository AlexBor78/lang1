cmake_minimum_required(VERSION 3.10)
project(Compiler)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(sources
    src/main.cpp
    src/cliparser.cpp
)

add_executable(${PROJECT_NAME} ${sources})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PUBLIC Lang)

# for breakpoints in code for debug
if(DEBUGGING)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUGGING)
endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
