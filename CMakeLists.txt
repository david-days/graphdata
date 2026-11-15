cmake_minimum_required(VERSION 3.25)
project(graphData LANGUAGES C)

include(GNUInstallDirs)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# C compiler settings (append — do not clobber distro/rpmbuild CFLAGS)
set(CMAKE_C_STANDARD 17)
add_compile_options(-Wall -Werror -Wno-format-truncation)

set(PROJECT_AUTHOR "David C. Days")
set(PROJECT_VENDOR "Silicon Holler")

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH  1)
string(TIMESTAMP PROJECT_VERSION_TWEAK %Y%m%d%H%M%S)

set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

add_subdirectory(src)

# Unit testing
enable_testing()
add_subdirectory(tests)
add_subdirectory(examples)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/packaging.cmake)

if( PRINT_ALL_VARS )
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endif()
