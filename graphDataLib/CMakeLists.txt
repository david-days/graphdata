cmake_minimum_required(VERSION 3.25)
project(graphData LANGUAGES C)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# C compiler settings
set(C_STANDARD 17)
set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-format-truncation")

set(PROJECT_AUTHOR "David C. Days")
set(PROJECT_VENDOR "Silicon Holler")

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH  0)
string(TIMESTAMP PROJECT_VERSION_TWEAK %Y%m%d%H%M%S)

set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

if ( RUN_LINTER )
    set(CMAKE_C_CLANG_TIDY clang-tidy -extra-arg=-Wno-unknown-warning-option)
endif( RUN_LINTER )

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    message(STATUS "Configuring for Linux")
    # Linux configuration files
    configure_file(conf.in/linux-x64-runtime.nuspec.in ${PROJECT_SOURCE_DIR}/output/linux-x64-runtime.nuspec)
    configure_file(conf.in/linux_runtime.csproj.in ${PROJECT_SOURCE_DIR}/output/linux_runtime.csproj)
    
elseif(CMAKE_SYSTEM_NAME MATCHES Windows)
    message(STATUS "Configuring for Win-x64")
    #Windows configuration files
    configure_file(conf.in/win-x64-runtime.nuspec.in ${PROJECT_SOURCE_DIR}/output/win-x64-runtime.nuspec)
    configure_file(conf.in/win_runtime.csproj.in ${PROJECT_SOURCE_DIR}/output/win_runtime.csproj)
endif()

add_subdirectory(src)

# Unit testing
enable_testing()
add_subdirectory(tests)

if( PRINT_ALL_VARS )
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endif()
