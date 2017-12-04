cmake_minimum_required(VERSION 3.8)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall -Werror -lcunit")

include(CTest)

set(SOURCES
        ${CMAKE_SOURCE_DIR}
        src
)

set(LIBS ${LIBS} ${PROJECT_NAME})

set(CMAKE_INCLUDE_PATH
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_BINARY_DIR}
        CMAKE_SYSTEM_INCLUDE_PATH
)

include_directories(
        src
        ../src
        ../include
)

add_executable(comptests src/comptests.c)
add_executable(inittests src/inittests.c)
add_executable(spatialtests src/spatialtests.c)
add_executable(hashtests src/hashtests.c)

enable_testing()

target_link_libraries(
        comptests
        ${LIBS}
)

target_link_libraries(
        inittests
        ${LIBS}
)

target_link_libraries(
        spatialtests
        ${LIBS}
)

target_link_libraries(
        hashtests
        ${LIBS}
)

add_test(comptests ${CMAKE_CURRENT_BINARY_DIR}/comptests)

add_test(inittests ${CMAKE_CURRENT_BINARY_DIR}/inittests)

add_test(spatialtests ${CMAKE_CURRENT_BINARY_DIR}/spatialtests)

add_test(hasthtests ${CMAKE_CURRENT_BINARY_DIR}/hashtests)