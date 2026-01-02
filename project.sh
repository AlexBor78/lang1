#!/usr/bin/bash

# defaults
LOC=0
RUN=0
TEST=0
HELP=0
BUILD=0
CLEAN=0
CLEAN_DOCS=0
GENERATE_DOCS=0

generate_docs() {
    echo "Generating documentation..."
    cmake --build build --target docs
    echo "Documentation generated in doc/api/generated"
}

build() {
    case $BUILD_TYPE in
        "Release") build_release ;;
        "Debug") build_debug ;;
    esac
}

build_release() {
    echo "Building Release version with $JOBS jobs..."
    mkdir -p build
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build --parallel $JOBS
    echo "Build completed!"
}

build_debug() {
    echo "Building Debug version with $JOBS jobs..."
    mkdir -p build
    cmake -B build -DCMAKE_BUILD_TYPE=Debug -DDEBUGGING=1
    cmake --build build --parallel $JOBS
    echo "Build completed!"
}

run() {
    ./build/src/compiler_app/Compiler
}

clean() {
    rm -rf build
    rm -rf .cache
    if [[ $CLEAN_DOCS -eq 1 ]]; then
        rm -rf docs/api/generated
    fi
}

print_help() {
    echo "USAGE: Usage: $0 [command] [--flags]"
    echo
    echo "Commands:"
    echo "build         - build the project,"
    echo "              have flags --debug and --release"
    echo "              by default used release"
    echo "              also have -j --jobs to specify working cores"
    echo "              by default use nproc"
    echo
    echo "clean         - clean generated files (after build or generated docs)"
    echo "              have one flag --docs to clean the auto-generated docs"
    echo
    echo "generate-docs - generating documentation"
}

test() {
    echo "testing is not supported now"
}

loc() {
    CLEAN_DOCS=1
    clean
    tree
    git ls-files | xargs wc -l
}

# process args
while [[ $# -gt 0 ]]; do
    case $1 in
        --help)
            HELP=1
            shift
            ;;
        build)
            BUILD=1
            BUILD_TYPE="Release"
            JOBS=$(nproc)
            shift
            ;;
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --release)
            BUILD_TYPE="Release" 
            shift
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        loc)
            LOC=1
            shift
            ;;
        test)
            TEST=1
            shift
            ;;
        run)
            RUN=1
            shift
            ;;
        clean)
            CLEAN=1
            shift
            ;;
        --docs)
            CLEAN_DOCS=1
            shift
            ;;
        generate-docs)
            GENERATE_DOCS=1
            shift
            ;;
        *)
            echo "Unknown option: $1"
            echo "Usage: $0 [command] [--flags]"
            echo "Try --help for help"
            exit 1
            ;;
    esac
done

if [[ $BUILD -eq 1 ]]; then
    build
fi

if [[ $RUN -eq 1 ]]; then
    run
fi

if [[ $GENERATE_DOCS -eq 1 ]]; then
    generate_docs
fi

if [[ $CLEAN -eq 1 ]]; then 
    clean
fi

if [[ $TEST -eq 1 ]]; then 
    test
fi

if [[ $LOC -eq 1 ]]; then
    loc
fi

if [[ $HELP -eq 1 ]]; then
    print_help
fi
