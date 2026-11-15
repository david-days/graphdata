# Run by cd into win build dir and running " cmake . --toolchain win-x64.cmake -B ./build-win-x64"
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/win-x64)

# the name of the target operating system
set(TARGET x86_64-w64-mingw32)
set(CMAKE_C_COMPILER_TARGET ${TARGET})
set(CMAKE_CXX_COMPILER_TARGET ${TARGET})
set (CMAKE_OS_NAME "Windows" CACHE STRING "Operating system name" FORCE)
set(CMAKE_SYSTEM_NAME "Windows")
set(CMAKE_SYSTEM_PROCESSOR "x86_64")
set(CMAKE_SYSTEM_VERSION "10")

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set (CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)


remove_definitions(
        __linux__
        __APPLE__
)

