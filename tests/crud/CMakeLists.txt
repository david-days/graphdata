add_executable(inittests
        inittests.c
)

target_link_libraries(inittests
        PUBLIC ${PROJECT_NAME}
)