set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/tests/bin)

set(TEST_DIR ${PROJECT_SOURCE_DIR}/tests)
set(dataDir ${TEST_DIR}/data)

add_compile_definitions(
        
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

add_link_options(
        -lcheck
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
#        URL https://some/url/to/data/file.zip
#        SOURCE_DIR ${dataDir}
#)
#FetchContent_MakeAvailable(testdata)

# Test executables
enable_testing()


# Math ops
add_test(NAME comptests COMMAND "comptests"
        WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)

# Utility functions
add_test(NAME hashtests COMMAND "hashtests"
        WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)

# Spatial ops and calculations
add_test(NAME spatialtests COMMAND "spatialtests"
        WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)

# CRUD group
add_test(NAME inittests COMMAND "inittests"
        WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)

# Graph manipulation
add_test(NAME graphtests COMMAND "graphtests"
        WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)
