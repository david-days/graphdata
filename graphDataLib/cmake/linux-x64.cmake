# Run by cd into linux build dir and running "cmake . --toolchain ./cmake/linux-x64.cmake -B ./build-linux-x64"
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/linux-x64)
set(CMAKE_INSTALL_PREFIX /usr)

add_definitions(
        -D__linux__
)

# Default to what cmake detects so we have a value of some sort
set(CMAKE_OS_NAME ${CMAKE_SYSTEM_NAME} CACHE STRING "Default operating system name" FORCE)
set(CMAKE_OS_VERSION "all" CACHE STRING "Default distribution version" FORCE)

# OS-specific detection based on https://gist.github.com/lbaehren/2413369
if (UNIX)

    find_file (OSRELEASE_FOUND os-release
            PATHS /etc
    )
    if (OSRELEASE_FOUND)
        # Read the content of /etc/os-release into a variable
        file(READ "/etc/os-release" OS_RELEASE_CONTENT)

        # Parse the content and extract the desired variables
        string(REGEX MATCH "[^_]ID=[\"A-Za-z0-9_\.]+" CMAKE_OS_NAME "${OS_RELEASE_CONTENT}")
        message(STRIP "${CMAKE_OS_NAME}" CMAKE_OS_NAME)
        string(REGEX REPLACE "[\" \t\r\n]" "" CMAKE_OS_NAME "${CMAKE_OS_NAME}")
        message(STATUS "CMAKE_OS_NAME: ${CMAKE_OS_NAME}")
        string(REGEX REPLACE "ID=" "" CMAKE_OS_NAME "${CMAKE_OS_NAME}")

        string(REGEX MATCH "VERSION_ID=[\"A-Za-z0-9_\.]+" CMAKE_OS_VERSION "${OS_RELEASE_CONTENT}")
        string(REGEX REPLACE "\"" "" CMAKE_OS_VERSION "${CMAKE_OS_VERSION}")
        string(REGEX REPLACE "VERSION_ID=" "" CMAKE_OS_VERSION "${CMAKE_OS_VERSION}")

        # Use the extracted variables
        message(STATUS "/etc/os-release: ${OS_RELEASE_CONTENT}")
        message(STATUS "CMAKE_OS_NAME: ${CMAKE_OS_NAME}")
        message(STATUS "CMAKE_OS_VERSION: ${CMAKE_OS_VERSION}")

    else (OSRELEASE_FOUND)

        ## Check for Debian GNU/Linux ________________
        find_file (DEBIAN_FOUND debian_version debconf.conf
                PATHS /etc
        )
        ##  Check for Fedora _________________________
        find_file (FEDORA_FOUND fedora-release
                PATHS /etc
        )
        ##  Check for RedHat _________________________
        find_file (REDHAT_FOUND redhat-release inittab.RH
                PATHS /etc
        )

        if (DEBIAN_FOUND)
            set (CMAKE_OS_NAME "debian" CACHE STRING "Operating system name" FORCE)
            file (STRINGS "/etc/debian_version" CMAKE_OS_VERSION)
            ## At its core Ubuntu is a Debian system, with
            ## a slightly altered configuration; hence from
            ## a first superficial inspection a system will
            ## be considered as Debian, which signifies an
            ## extra check is required.
            find_file (UBUNTU_EXTRA legal issue
                    PATHS /etc
            )

            if (UBUNTU_EXTRA)
                ## Scan contents of file
                file (STRINGS ${UBUNTU_EXTRA} UBUNTU_FOUND
                        REGEX Ubuntu
                )
                ## Check result of string search
                if (UBUNTU_FOUND)
                    set (CMAKE_OS_NAME "ubuntu" CACHE STRING "Operating system name" FORCE)
                    execute_process(
                            COMMAND source /etc/os-release && echo $VERSION_ID
                            OUTPUT_VARIABLE CMAKE_OS_VERSION
                    )
                    set (DEBIAN_FOUND FALSE)
                endif (UBUNTU_FOUND)
            endif (UBUNTU_EXTRA)
        elseif (FEDORA_FOUND)
            set (CMAKE_OS_NAME "fedora" CACHE STRING "Operating system name" FORCE)
            execute_process(
                    COMMAND source /etc/os-release && echo $VERSION_ID
                    OUTPUT_VARIABLE CMAKE_OS_VERSION
            )
        elseif (REDHAT_FOUND)
            set (CMAKE_OS_NAME "redhat" CACHE STRING "Operating system name" FORCE)
            file (STRINGS "/etc/redhat-release" CMAKE_OS_VERSION)
        endif (DEBIAN_FOUND)

    endif (OSRELEASE_FOUND)

endif (UNIX)