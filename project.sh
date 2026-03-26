#!/usr/bin/env bash

# defaults
declare -A cmd
cmd[name]=""
cmd[flags]=""

# toolchain

has_flag() {
    local flag="$1"
    [[ " ${cmd[flags]} " == *" ${flag} "* ]]
}

# logic functoins

build() {
	mkdir -p build

	if has_flag "--release"; then
		# release configuration
    echo "Building Release version with $JOBS jobs..."
    cmake -B build -DCMAKE_BUILD_TYPE=Release
	elif has_flag "--debug"; then
		# debug configuration
    echo "Building Debug version with $JOBS jobs..."
    cmake -B build -DCMAKE_BUILD_TYPE=Debug -DDEBUGGING=1
	else
		# default configuration (release)
    echo "Building Release version with $JOBS jobs..."
    cmake -B build -DCMAKE_BUILD_TYPE=Release
	fi


	if ! cmake --build build --parallel "$(nproc)"; then
		echo "Build failed"
		exit 1
	fi
  echo "Build completed!"

	cmake --install build --prefix "$(pwd)"
}

gen-docs() {
    echo "Generating documentation..."
    cmake --build build --target docs
    echo "Documentation generated in doc/api/generated"
}

run() {
    ./build/src/compiler_app/Compiler
}

clean() {
    rm -rf build .cache bin
    if has_flag "--docs"; then
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
	# cleanup
	cmd[flags]+=" --docs"
  clean
	# loc
  tree
  git ls-files | xargs wc -l
}

parse-args() {
	#local done-flags=0
	while [[ $# -gt 0 ]]; do
		local arg="$1"
		
		# if its --
		# we dont have any args yet, so just ignore anything after
		if [ "${arg}" = "--" ]; then
		#	local done-flags=1;
		#	shift 1
		#	continue
			shift 1
			break
		fi

		# if its flag (started with -)
		if [ "${arg:0:1}" = "-" ]; then
			cmd["flags"]+=" $1"
			shift 1
			continue
		fi

		# if it is command (just word)
		
		# if command is not empty (cmd overriding)
		if [ -n "${cmd["name"]}" ]; then
			echo "error commnd overriding."
			echo "use --help for help"
			exit 1
		fi

		cmd["name"]="$1"

		# to don't loop forever
		shift 1
	done
}

main() {
	# parsing args
	parse-args "$@"


	#	todo: help

	if [ "${cmd["name"]}" = "build" ]; then
		build
	elif [ "${cmd["name"]}" = "run" ]; then
		run
	elif [ "${cmd["name"]}" = "clean" ]; then
		clean
	elif [ "${cmd["name"]}" = "gen-docs" ]; then
		gen-docs
	fi


	
	
#	if [[ $TEST -eq 1 ]]; then 
#	    test
#	fi
#	
#	if [[ $LOC -eq 1 ]]; then
#	    loc
#	fi
#	
#	if [[ $HELP -eq 1 ]]; then
#	    print_help
#	fi
}

main "$@"
