cmake_minimum_required(VERSION 3.20)

project(OpcUaRegler
    VERSION 0.1.0
    DESCRIPTION "EUROMAP 83 oriented OPC UA working point controller microservice"
    LANGUAGES CXX)

option(OPCUAREGLER_WITH_OPEN62541 "Build the open62541 OPC UA backend" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(opcuaregler_core
    src/alarm.cpp
    src/config.cpp
    src/controller.cpp
    src/euromap83_model.cpp
    src/opcua_memory_backend.cpp
    src/process_simulation.cpp
)

target_include_directories(opcuaregler_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(opcuaregler_core PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(opcuaregler_core PRIVATE /W4)
else()
    target_compile_options(opcuaregler_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(OPCUAREGLER_WITH_OPEN62541)
    find_package(open62541 CONFIG REQUIRED)
    target_sources(opcuaregler_core PRIVATE src/opcua_open62541_backend.cpp)
    if(TARGET open62541::open62541)
        target_link_libraries(opcuaregler_core PUBLIC open62541::open62541)
    elseif(TARGET open62541)
        target_link_libraries(opcuaregler_core PUBLIC open62541)
    else()
        message(FATAL_ERROR "open62541 package found, but no known CMake target is available")
    endif()
    target_compile_definitions(opcuaregler_core PUBLIC OPCUAREGLER_WITH_OPEN62541=1)
endif()

add_executable(opcuaregler src/main.cpp)
target_link_libraries(opcuaregler PRIVATE opcuaregler_core)

include(CTest)
if(BUILD_TESTING)
    add_executable(opcuaregler_tests
        tests/controller_tests.cpp
    )
    target_link_libraries(opcuaregler_tests PRIVATE opcuaregler_core)
    add_test(NAME opcuaregler_tests COMMAND opcuaregler_tests)
endif()
