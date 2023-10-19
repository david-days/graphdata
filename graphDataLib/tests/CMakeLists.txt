set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/tests/bin)

set(TEST_DIR ${PROJECT_SOURCE_DIR}/tests)
set(dataDir ${TEST_DIR}/data)

add_compile_definitions(
        -DUNICODE
        -D_UNICODE
        -DUNIX
        -D_REENTRANT
        -D_LARGEFILE64_SOURCE
)

add_definitions(
        -DARCH=64
        -DCFG=Release
)

link_libraries(
        ${PROJECT_NAME}
)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
)

# Builds for test binaries
add_subdirectory(crud)
add_subdirectory(graph)
add_subdirectory(math)
add_subdirectory(spatial)
add_subdirectory(util)

# Retrieve test data
#include(FetchContent)
#set(FETCHCONTENT_QUIET FALSE)
#FetchContent_Declare(testdata
#        URL https://maplarge-public.s3.amazonaws.com/ImportTestData/FileGDB/filegdb-data-20230912.zip
#        SOURCE_DIR ${dataDir}
#)
#FetchContent_MakeAvailable(testdata)

# Test executables
enable_testing()

add_test(NAME LibFunctionTest COMMAND "LibFunctionTest"
        WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)