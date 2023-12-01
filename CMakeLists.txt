cmake_minimum_required(VERSION 3.22)
project(userver-demo LANGUAGES CXX)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

include(cmake/conan.cmake)
conan_cmake_install(
    PATH_OR_REFERENCE ${CMAKE_SOURCE_DIR}
    INSTALL_FOLDER ${CMAKE_BINARY_DIR}
    BUILD missing
    ENV "CONAN_CMAKE_GENERATOR=${CMAKE_GENERATOR}"
    CONF "tools.cmake.cmaketoolchain:generator=${CMAKE_GENERATOR}"
    PROFILE "userver-demo/default"
    PROFILE_BUILD "userver-demo/default"
    PROFILE_HOST "userver-demo/default"
)
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

find_package(userver 1.0.0 EXACT REQUIRED)

add_grpc_library(userver-demo-proto PROTOS samples/greeter.proto)

add_executable(${PROJECT_NAME}
    src/components/samples/greeter_service_component.cpp
    src/components/samples/greeter_service_component.h
    src/main.cpp
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        userver-demo-proto
)
target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Werror
        -Wall
        -Wextra
        -Wassign-enum
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wextra-semi
        -Wsign-compare
        -Wsign-conversion
        -pedantic
        -pedantic-errors
)
