#!/usr/bin/bash

# defaults
BUILD_TYPE="Release"
CMAKE_DEFINES=""

# process args
while [[ $# -gt 0 ]]; do
    case $1 in
        debug)
            BUILD_TYPE="Debug"
            CMAKE_DEFINES="-DDEBUGGING=1"
            shift
            ;;
        release)
            BUILD_TYPE="Release" 
            shift
            ;;
        --docs)
            GENERATE_DOCS=1
            shift
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        *)
            echo "Unknown option: $1"
            echo "Usage: $0 [debug|release] [-j N]"
            exit 1
            ;;
    esac
done

# jobs
if [[ -z "$JOBS" ]]; then
    JOBS=$(nproc)
fi

# building...
echo "Building $BUILD_TYPE version with $JOBS jobs..."

mkdir -p build
cmake -B build -DCMAKE_BUILD_TYPE=$BUILD_TYPE $CMAKE_DEFINES
cmake --build build --parallel $JOBS

if [[ $GENERATE_DOCS -eq 1 ]]; then
    echo "Generating documentation..."
    cmake --build build --target docs
    echo "Documentation generated in docs/api/html/"
fi


echo "Build completed!"
