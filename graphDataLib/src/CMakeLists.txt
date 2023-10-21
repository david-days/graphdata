add_library(${PROJECT_NAME} SHARED
        graphInit.c
        impl/arraygraph.c
        impl/arrayops.c
        impl/hashgraph.c
        impl/hashops.c
        impl/linkgraph.c
        impl/linkops.c
        impl/sharedmemgraph.c
        impl/sharedmemops.c
        impl/sharedmmapgraph.c
        impl/sharedmmapops.c
        util/cartesian.c
        util/crudops.c
        util/graphcomp.c
        util/hashes.c
)
set(BUILD_SHARED_LIBS 1)

# Configure the directories to search for header files.
target_include_directories(${PROJECT_NAME} PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/impl
        ${PROJECT_SOURCE_DIR}/include/util
)
# Set the version property.
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
# Set the shared object version property to the project's major version.
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
# Set the public header property to the one with the actual API.
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${PROJECT_SOURCE_DIR}/include/graphData.h)


include(${PROJECT_SOURCE_DIR}/cmake/packaging.cmake)


install(TARGETS ${PROJECT_NAME}
        DESTINATION "lib"
)

install(FILES
        ${PROJECT_SOURCE_DIR}/include/graphData.h
        ${PROJECT_SOURCE_DIR}/include/graphInit.h
        ${PROJECT_SOURCE_DIR}/include/graphOps.h
        DESTINATION "include"
)

configPackaging()
