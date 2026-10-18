macro(configDocs)
    # Based on article at https://vicrucann.github.io/tutorials/quick-cmake-doxygen/
    # Docs are opt-in: cmake -DBUILD_DOC=ON, or build scripts -d / -d flag
    option(BUILD_DOC "Build documentation" OFF)

    if (BUILD_DOC)
        find_package(Doxygen)
        if (DOXYGEN_FOUND)
            set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/conf.in/Doxyfile.in)
            set(DOXYGEN_OUT ${PROJECT_SOURCE_DIR}/output/Doxyfile)

            configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
            message(STATUS "Doxygen documentation target enabled (make docs)")

            add_custom_target( docs
                    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Generating graphData docs with Doxygen"
                    VERBATIM )
        else (DOXYGEN_FOUND)
            message(WARNING "BUILD_DOC=ON but Doxygen was not found")
        endif (DOXYGEN_FOUND)
    endif (BUILD_DOC)

endmacro(configDocs)
