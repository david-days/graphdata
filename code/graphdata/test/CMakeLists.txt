cmake_minimum_required(VERSION 3.8)

include(CTest)

set(SOURCES
        ${CMAKE_SOURCE_DIR}
        src
)

set(CMAKE_INCLUDE_PATH
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_BINARY_DIR}
        CMAKE_SYSTEM_INCLUDE_PATH
)

add_executable(comptests src/comptests.c)

enable_testing()

add_test(comptests ${EXECUTABLE_OUTPUT_PATH}/comptests)