# Run by cd into linux build dir and running "cmake . --toolchain ./cmake/linux-x64.cmake -B ./build-linux-x64"
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/linux-x64)
set(CMAKE_INSTALL_PREFIX /usr)

add_definitions(
        -D__linux__
)

# OS-specific detection based on https://gist.github.com/lbaehren/2413369 
if (UNIX)

    if (APPLE)

        set (CMAKE_OS_NAME "OSX" CACHE STRING "Operating system name" FORCE)

    else (APPLE)

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
            set (CMAKE_OS_NAME "Debian" CACHE STRING "Operating system name" FORCE)

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
                    set (CMAKE_OS_NAME "Ubuntu" CACHE STRING "Operating system name" FORCE)
                    set (DEBIAN_FOUND FALSE)
                endif (UBUNTU_FOUND)
            endif (UBUNTU_EXTRA)
        elseif (FEDORA_FOUND)
            set (CMAKE_OS_NAME "Fedora" CACHE STRING "Operating system name" FORCE)
        elseif (REDHAT_FOUND)
            set (CMAKE_OS_NAME "RedHat" CACHE STRING "Operating system name" FORCE)
        endif (DEBIAN_FOUND)

    endif (APPLE)

endif (UNIX)