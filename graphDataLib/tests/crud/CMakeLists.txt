add_executable(inittests
        inittests.c
)

target_link_libraries(inittests
        ${PROJECT_NAME}
)