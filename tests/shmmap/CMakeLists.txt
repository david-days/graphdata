add_executable(shmmaptests
        shmmaptests.c
)

target_link_libraries(shmmaptests
        PUBLIC ${PROJECT_NAME}
)