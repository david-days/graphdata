add_executable(spatialtests
        spatialtests.c
)

target_link_libraries(spatialtests
        ${PROJECT_NAME}
)