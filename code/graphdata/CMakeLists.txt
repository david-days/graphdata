project(graphdata C)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall -Werror -Wno-format-truncation")

set(PROJECT_AUTHOR "David C. Days")
set(PROJECT_VENDOR "Silicon Holler")

set(GRAPHDATA_VERSION_MAJOR 1)
set(GRAPHDATA_VERSION_MINOR 0)
set(GRAPHDATA_VERSION_PATCH 0)
set(GRAPHDATA_VERSION ${GRAPHDATA_VERSION_MAJOR}.${GRAPHDATA_VERSION_MINOR}.${GRAPHDATA_VERSION_PATCH})

configure_file(
        "include/graphdata.h.in"
        "${PROJECT_BINARY_DIR}/graphdata.h"
)

set(PROJECT_SOURCE_DIR src)

set(CMAKE_INCLUDE_PATH
    include
    CMAKE_SYSTEM_INCLUDE_PATH
    ${PROJECT_BINARY_DIR}
)

set(CMAKE_INSTALL_PREFIX /usr/share/graphdata)

set(SOURCE_FILES
        #HEADERS
        include/graphinit.h
        include/util/hashes.h
        include/util/graphcomp.h

        #CODE
        src/graphinit.c
        src/util/hashes.h
        src/util/graphcomp.c

        #CONFIG
        conf/graphdata.conf
)

include_directories(
        include
)

add_library(${PROJECT_NAME} SHARED
        #Headers
        ${PROJECT_BINARY_DIR}/graphdata.h
        include/graphinit.h
        include/graphops.h
        include/util/hashes.h
        include/util/graphcomp.h

        #Sources
        src/graphinit.c
        src/util/hashes.c
        src/util/graphcomp.c

)

set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${GRAPHDATA_VERSION}
        SOVERSION ${GRAPHDATA_VERSION_MAJOR}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_INCLUDE_PATH})

target_link_libraries(${PROJECT_NAME})

add_subdirectory(test)


#Installation and packaging

install(
        TARGETS ${PROJECT_NAME}
        LIBRARY
        DESTINATION lib
        COMPONENT libraries
)
install(
        FILES
            ${PROJECT_BINARY_DIR}/graphdata.h
            include/graphinit.h
            include/graphops.h
        DESTINATION include
        COMPONENT libraries
)
install(
        FILES
            include/util/hashes.h
            include/util/graphcomp.h
        DESTINATION include/util
        COMPONENT libraries
)
install(
        FILES conf/graphdata.conf
        DESTINATION /etc/ld.so.conf.d
        COMPONENT libraries
)




#CPack RPM Generation
set(CPACK_PACKAGE_VERSION_MAJOR ${GRAPHDATA_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${GRAPHDATA_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${GRAPHDATA_VERSION_PATCH})
set(CPACK_GENERATOR "RPM")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT ${PROJECT_AUTHOR})
set(CPACK_PACKAGE_VENDOR ${PROJECT_VENDOR})
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

set(CPACK_COMPONENTS_ALL libraries)
set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Basic graph data and operations shared library")
set(CPACK_COMPONENT_LIBRARIES_GROUP "libs")
set(CPACK_COMPONENT_DEVEL_GROUP "devel")

#RPM-specific settings
set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_RPM_PACKAGE_SUMMARY "Graph structures and operations shared library")
set(CPACK_RPM_PACKAGE_DESCRIPTION "Shared libraries and headers for graph functions")
set(CPACK_RPM_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
set(CPACK_RPM_PACKAGE_LICENSE "GPLv3+")
set(CPACK_RPM_PACKAGE_URL "https://github.com/david-days/graphdata")
set(CPACK_RPM_DEFAULT_FILE_PERMISSIONS
        OWNER_READ
        OWNER_WRITE
        GROUP_READ
        WORLD_READ
)
set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/scripts/UN_INSTALL)
set(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/scripts/UN_INSTALL)
set(CPACK_RPM_SPEC_INSTALL_POST "/bin/true")
#Exclude common directory creation to avoid conflicts
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION
        /etc/ld.so.conf.d
)
set(CPACK_RPM_PACKAGE_REQUIRES
        #"sg3_utils, lsscsi"
)


set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")



INCLUDE(CPack)

#Documentation build.
#Original config from Victoria Rudakova's page at https://vicrucann.github.io/tutorials/quick-cmake-doxygen/
option(BUILD_DOC "Build documentation" ON)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/../../docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
else(DOXYGEN_FOUND)
    message("Doxygen must be installed to generate the documentation")
endif(DOXYGEN_FOUND)