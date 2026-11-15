add_executable(comptests
        comptests.c
)

target_link_libraries(comptests
        PUBLIC ${PROJECT_NAME}
)