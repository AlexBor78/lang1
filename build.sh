#!/usr/bin/bash

mkdir -p build
cmake build
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
