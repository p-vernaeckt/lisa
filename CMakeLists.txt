cmake_minimum_required(VERSION 3.16)

# Conan inside CMake
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake" TLS_VERIFY ON)
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build options (Debug, Release, RelWithDebInfo or MinSizeRel)." FORCE)
endif(NOT CMAKE_BUILD_TYPE)


# ----------------------------------------------------------
# Commons
# ----------------------------------------------------------
set(MAIN_PROJECT Lisa)
set(TESTS tests)

set(LISA_VERSION_MAJOR 0)
set(LISA_VERSION_MINOR 0)
set(LISA_VERSION_PATCH 1)

configure_file(
    ./include/config.h.in
    config.h
)
# project settings
project(
    ${MAIN_PROJECT}
)

conan_cmake_autodetect(settings)
conan_cmake_install(
    PATH_OR_REFERENCE ..
    BUILD missing
    CMAKE_GENERATOR cmake
    REMOTE conancenter
    SETTINGS ${settings}
)

# some global variables
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-fpermissive")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# source files
set(SRCS
    src/main.cpp
)

include_directories(
    include
    build
)

# ----------------------------------------------------------
# Main project
# ----------------------------------------------------------
set(CONAN_DISABLE_CHECK_COMPILER true)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(
    ${MAIN_PROJECT}
    ${SRCS}
)

target_link_libraries(
    ${MAIN_PROJECT}
    ${CONAN_LIBS}
)
