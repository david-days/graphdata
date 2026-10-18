add_executable(shmemtests
        shmemtests.c
)

target_link_libraries(shmemtests
        PUBLIC ${PROJECT_NAME}
)