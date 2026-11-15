add_executable(bfs_demo
        bfs_demo.c
        bfs_shortest_path.c
)

target_include_directories(bfs_demo PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(bfs_demo PRIVATE
        ${PROJECT_NAME}
)

add_test(NAME bfs_demo COMMAND bfs_demo)
