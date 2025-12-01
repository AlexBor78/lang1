cmake_minimum_required(VERSION 3.10)
project(Common)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(sources
    src/streams/istream.cpp
    src/streams/ostream.cpp
    src/streams/stream.cpp
    src/utils/logger.cpp
    src/diagnostic/diagnostic_builder.cpp
)

add_library(${PROJECT_NAME} STATIC ${sources})
target_include_directories(${PROJECT_NAME} PUBLIC include)
