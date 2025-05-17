
macro(configPackaging)

    # General reusable settings
    set(GRAPHDATA_PKG_VENDOR "Mad Goat")
    set(GRAPHDATA_PKG_NAME "graphdata")
    set(GRAPHDATA_PKG_MAINTAINER "David C Days <david.c.days@gmail.com>")
    set(GRAPHDATA_PKG_SUMMARY "Efficient implementation for common graph data operations")
    set(GRAPHDATA_PKG_DESCR "Native library to provide general graph data operations")
    set(GRAPHDATA_PKG_VERSION ${PROJECT_VERSION})
    set(GRAPHDATA_PROJECT_HOME "https://github.com/david-days/graphdata")
    
    # packaging specific settings
    set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
    set(CPACK_PACKAGE_DIRECTORY "${PROJECT_SOURCE_DIR}/output/${CMAKE_OS_NAME}/${CMAKE_OS_VERSION}/${CMAKE_SYSTEM_PROCESSOR}")
    set(CPACK_PACKAGE_VERSION ${GRAPHDATA_PKG_VERSION})
    set(CPACK_PACKAGE_NAME ${GRAPHDATA_PKG_NAME})
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}.${CMAKE_OS_NAME}.${CMAKE_OS_VERSION}.${CMAKE_SYSTEM_PROCESSOR}")
    set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-src")
    set(CPAKE_VERBATIM_VARIABLES TRUE)
    set(CPACK_SOURCE_IGNORE_FILES
            /\\..*  # any file/directory starting with .
            /.*\\.yml
            /.*\\.gz
            /.*\\.zip
            /.*build.*/
            \\.deps
            /autogen\\.sh
            /autom4te\\.cache
            /CODE_OF_CONDUCT.md
            /CONTRIBUTING.md
            /disabled_workflows/
            /Dockerfile
            /docs/
            /Doxyfile
            /HOWTO-RELEASE
            /m4/lt*
            /m4/libtool*
            /media/
            /schemas/
            /scripts/
            /test/fuzzers/
            /test/gigs/.*gie\\.failing
            /test/postinstall/
            /travis/
            ${PROJECT_BINARY_DIR}
    )

    if (CMAKE_OS_NAME MATCHES fedora OR CMAKE_OS_NAME MATCHES redhat OR CMAKE_OS_NAME MATCHES rhel)
        message("Building on Fedora/Redhat")
        set(CPACK_GENERATOR "RPM;TGZ")
        set(CPACK_RPM_PACKAGE_SUMMARY ${GRAPHDATA_PKG_SUMMARY})
        set(CPACK_RPM_PACKAGE_DESCRIPTION ${GRAPHDATA_PKG_DESCR})
        set(CPACK_RPM_PACKAGE_VENDOR ${GRAPHDATA_PKG_VENDOR})
        set(CPACK_RPM_PACKAGE_VERSION ${GRAPHDATA_PKG_VERSION})
        set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
        set(CPACK_RPM_PACKAGE_RELEASE 1)
        # set(CPACK_RPM_PACKAGE_LICENSE "")
        set(CPACK_RPM_PACKAGE_GROUP "Applications/Engineering")
        set(CPACK_RPM_PACKAGE_URL ${GRAPHDATA_PROJECT_HOME})
        
    elseif(CMAKE_OS_NAME MATCHES Debian OR CMAKE_OS_NAME MATCHES Ubuntu)
        
        set(CPACK_GENERATOR "DEB;TGZ")
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${GRAPHDATA_PKG_MAINTAINER})
        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${GRAPHDATA_PKG_SUMMARY})
        set(CPACK_PACKAGE_DESCRIPTION ${GRAPHDATA_PKG_DESCR})
        set(CPACK_DEBIAN_PACKAGE_DEPENDS libc6 (>= 2.7-18))
        # set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA ${CMAKE_CURRENT_SOURCE_DIR}/postinst)
        set(CPACK_DEBIAN_PACKAGE_SECTION Libraries)
        set(CPACK_DEBIAN_PACKAGE_VERSION ${GRAPHDATA_PKG_VERSION})
        set(CPACK_DEBIAN_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_OS_NAME}-${CMAKE_OS_VERSION}-${CMAKE_SYSTEM_PROCESSOR}.deb")

    elseif(CMAKE_OS_NAME MATCHES OSX)
        # TODO: OSX packaging config
        message("No OSX packaging yet!")
    elseif(CMAKE_OS_NAME MATCHES Windows)
        set(CPACK_WIX_PRODUCT_GUID fdf63aa6-327e-11f0-95bf-e4a8dfc2105c)
        set(CPACK_WIX_UPGRADE_GUID fdf63aa6-327e-11f0-95bf-e4a8dfc2105c)
        set(CPACK_WIX_CMAKE_PACKAGE_REGISTRY ${GRAPHDATA_PKG_NAME})
        set(CPACK_WIX_ROOT_FEATURE_DESCRIPTION ${GRAPHDATA_PKG_DESCR})
        set(CPACK_WIX_ARCHITECTURE x64)
        # set(CPACK_WIX_UI_BANNER 493x58.bmp)
        # set(CPACK_WIX_UI_DIALOG 498x312.bmp)
        # set(CPACK_WIX_PRODUCT_ICON custom.ico)
    endif()

    include(CPack)
endmacro()