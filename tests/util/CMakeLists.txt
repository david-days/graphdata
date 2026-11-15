add_executable(hashtests
        hashtests.c
)

target_link_libraries(hashtests
        PUBLIC ${PROJECT_NAME}
)