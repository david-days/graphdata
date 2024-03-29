macro(configDocs)
    # Based on article at https://vicrucann.github.io/tutorials/quick-cmake-doxygen/
    # first we can indicate the documentation build as an option and set it to ON by default
    option(BUILD_DOC "Build documentation" ON)

    # check if Doxygen is installed
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        # set input and output files
        set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/conf.in/Doxyfile.in)
        set(DOXYGEN_OUT ${PROJECT_SOURCE_DIR}/output/Doxyfile)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # note the option ALL which allows to build the docs together with the application
        add_custom_target( docs
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating graphData docs with Doxygen"
                VERBATIM )
    else (DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)

endmacro(configDocs)