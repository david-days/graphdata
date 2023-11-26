add_executable(hashtests
        hashtests.c
)

add_executable(strtests
    str_tests.c
)

target_link_libraries(hashtests
        PUBLIC ${PROJECT_NAME}
)

target_link_libraries(strtests 
        PUBLIC ${PROJECT_NAME}
)