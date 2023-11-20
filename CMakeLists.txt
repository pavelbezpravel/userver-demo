cmake_minimum_required(VERSION 3.16)
project(userver-demo LANGUAGES CXX)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

include(cmake/conan.cmake)

conan_cmake_autodetect(settings)
conan_cmake_install(
    PATH_OR_REFERENCE ${CMAKE_SOURCE_DIR}
    INSTALL_FOLDER ${CMAKE_BINARY_DIR}
    BUILD missing
    REMOTE conancenter
    SETTINGS ${settings}
    ENV "CONAN_CMAKE_GENERATOR=${CMAKE_GENERATOR}"
    CONF "tools.cmake.cmaketoolchain:generator=${CMAKE_GENERATOR}"
)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(cmake/userver.cmake)

set(target_name userver-demo)
set(source_list
    src/components/samples/greeter_client_component.cpp
    src/components/samples/greeter_client_component.h
    src/components/samples/greeter_http_handler_component.cpp
    src/components/samples/greeter_http_handler_component.h
    src/components/samples/greeter_service_component.cpp
    src/components/samples/greeter_service_component.h
    src/main.cpp
)

include(GrpcTargets)
add_grpc_library(${target_name}-proto PROTOS samples/greeter.proto)

add_executable(${target_name} ${source_list})
target_link_libraries(${target_name}
    PRIVATE
        ${target_name}-proto
)
