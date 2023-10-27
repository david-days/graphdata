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

# Immediate action flags
while getopts ":cithvxpd" option; do
    case $option in
        c)
        	# [c]lean
        	echo cleaning...
        	rm -rvf "${LINUX_BUILD_DIR}"/*
            rm -rvf "${BINARY_OUT_DIR}"/*
            rm -rvf "${TEST_BINARIES}"/*
            rm -rvf "${CMAKE_BUILD_DIR}"/*
      		exit 0
        	;;
     	i)
     		# [i]install
     		cd "${LINUX_BUILD_DIR}"
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
  		h)
  			# [h]elp
  			echo "Usage: ./buildLinux.sh [-c | -i | -t | -v | -h]"
  			echo "	No args  (Re)Build"
  			echo "	-c Clean.  Clean output folders and exit"
  			echo "	-i Install.  Build and install"
  			echo "	-t Test.  Build and run tests, then exit"
  			echo "	-v Verbose.  Print out CMAKE build variables and exit"
  			echo "	-x Extra verbose.  Print all CMAKE build variables used"
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

cmake . --toolchain "${LINUX_TOOLCHAIN}" -B "${LINUX_BUILD_DIR}" -DPRINT_ALL_VARS="${OUTPUT_ALL_VARS}" && cd "${LINUX_BUILD_DIR}" && make all

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
