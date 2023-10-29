#!/bin/bash

PROJECT_DIR=$(pwd)

LINUX_BUILD_DIR=${PROJECT_DIR}/build-linux-x64
CMAKE_BUILD_DIR=${PROJECT_DIR}/cmake-build-debug
BINARY_OUT_DIR=${PROJECT_DIR}/output
TEST_BINARIES=${PROJECT_DIR}/tests/bin
LINUX_TOOLCHAIN=${PROJECT_DIR}/cmake/linux-x64.cmake

RUN_TESTS=""
OUTPUT_VARS="false"
OUTPUT_ALL_VARS=0
PACKAGE="false"
GEN_DOCS=0
LINTER=0

# Immediate action flags
while getopts ":cithvxpdl" option; do
    case $option in
        c)
        	# [c]lean
        	echo cleaning...
        	# shellcheck disable=SC2115
        	rm -rvf "${LINUX_BUILD_DIR}"/*
            # shellcheck disable=SC2115
            rm -rvf "${BINARY_OUT_DIR}"/*
            # shellcheck disable=SC2115
            rm -rvf "${TEST_BINARIES}"/*
            # shellcheck disable=SC2115
            rm -rvf "${CMAKE_BUILD_DIR}"/*
      		exit 0
        	;;
     	i)
     		# [i]install
     		cd "${LINUX_BUILD_DIR}" || exit 1
     		make install
     		exit 0
     		;;
   		t)
   			RUN_TESTS="TEST"
   			;;
  		p)
  			# [p]ackage
  			PACKAGE="true"
  			;;
  		v)
  			OUTPUT_VARS="true"
  			;;
 		x)
 			# extra verbose
 			OUTPUT_ALL_VARS=1
 			;;
		d)
			# [d]ocumentation
			GEN_DOCS=1
			;;
		l)
			# [l]inter
			LINTER=1
			;;
  		h)
  			# [h]elp
  			echo "Usage: ./buildLinux.sh [-c | -i | -t | -v | -h]"
  			echo "	No args  (Re)Build"
  			echo "	-c Clean.  Clean output folders and exit"
  			echo "	-i Install.  Build and install"
  			echo "	-t Test.  Build and run tests, then exit"
  			echo "	-v Verbose.  Print out CMAKE build variables and exit"
  			echo "	-x Extra verbose.  Print all CMAKE build variables used"
  			echo "  -p Package.  Create install packages according to the platform and configuration."
  			echo "  -d Generate documentation.  Create the doxygen output for this code"
  			echo "  -l Linter.  Run the clang-tidy linter"
  			echo "	-h Help.  Print this help and exit"
  			exit 0
  			;;
        *)
        	;;
    esac
done

if [ ! -d "${LINUX_BUILD_DIR}" ]; then
  mkdir "${LINUX_BUILD_DIR}"
fi

if [ "${OUTPUT_VARS}" == "true" ]; then
	cmake . -LH --toolchain "${LINUX_TOOLCHAIN}" -B "${LINUX_BUILD_DIR}"
	exit 0
fi

cmake . --toolchain "${LINUX_TOOLCHAIN}" -B "${LINUX_BUILD_DIR}" -DPRINT_ALL_VARS="${OUTPUT_ALL_VARS}"  -DRUN_LINTER="${LINTER}" && cd "${LINUX_BUILD_DIR}" && make all

if [ ${GEN_DOCS} == 1 ]; then
	make docs
fi

# From here on we're in the build directory unless other actions are taken
if [ "${RUN_TESTS}" == "TEST" ]; then
	make test
fi

if [ "${PACKAGE}" == "true" ]; then
	# Native installer for this platform
	make package
	# Native runtime NuGet package
	dotnet restore "${BINARY_OUT_DIR}/linux_runtime.csproj"
	dotnet pack "${BINARY_OUT_DIR}/linux_runtime.csproj" --include-symbols --no-build -o "${BINARY_OUT_DIR}/nupkgs" --runtime linux-x64 -c Release

fi
