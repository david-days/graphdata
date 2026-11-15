add_executable(graphtests
        graphtests.c
)

target_link_libraries(graphtests
        PUBLIC ${PROJECT_NAME}
)