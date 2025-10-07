cmake_minimum_required(VERSION 3.24)
project(lichens_cpp_project LANGUAGES CXX)

cmake_policy(SET CMP0135 NEW)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

# CMake options to enable/disable modules
set(BUILD_LICHENS_CPP_LOG ON CACHE BOOL "Build the Log module.")
option(BUILD_LICHENS_CPP_CONFIG "Build the Config module." ON)
option(BUILD_LICHENS_CPP_LAUNCHER "Build the Launcher module." ON)
option(BUILD_LICHENS_CPP_MQTT "Build the MQTT module." ON)
option(BUILD_LICHENS_CPP_TESTS "Build the tests tree." ON)

set(SOURCES
    src/stringutil.cpp
)

set(LIBRARY_USED
)

set(INCLUDES_PUBLIC
    include
)

set(INCLUDES_PRIVATE
)

# spdlog
if(BUILD_LICHENS_CPP_LOG)
    find_package(fmt REQUIRED)
    find_path(SPDLOG_INCLUDE_DIR spdlog/spdlog.h)
    if(NOT SPDLOG_INCLUDE_DIR)
        message(FATAL_ERROR "spdlog headers not found. Please install spdlog.")
    endif()
    list(APPEND LIBRARY_USED
        fmt::fmt
    )
    list(APPEND SOURCES 
        src/log/Logger.cpp
    )
    list(APPEND INCLUDES_PRIVATE ${SPDLOG_INCLUDE_DIR})
    add_definitions(-DBUILD_LICHENS_CPP_LOG)
endif()

if(BUILD_LICHENS_CPP_CONFIG)
    message(STATUS "Building Lichens-CPP Config module.")
    find_package(nlohmann_json REQUIRED)
    list(APPEND SOURCES 
        src/config/AppConfig.cpp
        src/config/Config.cpp
        src/config/ConfigValue.cpp
    )
    list(APPEND LIBRARY_USED
        nlohmann_json::nlohmann_json
    )
    add_definitions(-DBUILD_LICHENS_CPP_CONFIG)
endif()

if(BUILD_LICHENS_CPP_LAUNCHER)
    message(STATUS "Building Lichens-CPP Launcher module.")
    list(APPEND SOURCES 
        src/launcher/LaunchProcess.cpp
    )
    add_definitions(-DBUILD_LICHENS_CPP_LAUNCHER)
endif()

if(BUILD_LICHENS_CPP_MQTT)
    message(STATUS "Building Lichens-CPP MQTT module.")
    
    # Paho MQTT C++ library must be install in the system from https://github.com/eclipse/paho.mqtt.cpp.git
    find_package(PahoMqttCpp REQUIRED)

    list(APPEND SOURCES 
        src/mqtt/MqttHelper.cpp
    )
    list(APPEND LIBRARY_USED
        PahoMqttCpp::paho-mqttpp3
    )
    add_definitions(-DBUILD_LICHENS_CPP_MQTT)
endif()

# Library source files
add_library(lichens_cpp SHARED 
    ${SOURCES}
)

# add include directories
target_include_directories(lichens_cpp 
    PRIVATE 
    ${INCLUDES_PRIVATE}
    PUBLIC
    ${INCLUDES_PUBLIC}
)

# Link libraries
target_link_libraries(lichens_cpp PRIVATE  
    ${LIBRARY_USED}
)

# Unit Tests
if(BUILD_LICHENS_CPP_TESTS)
    add_definitions(-DBUILD_LICHENS_CPP_TESTS)
    message(STATUS "Building Lichens-CPP tests.")

    # GoogleTest dependencies
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(test_lichens_cpp
        tests/main.cpp
        tests/config/test_Config.cpp
        tests/config/test_ConfigValue.cpp
    )

    include_directories(${GTEST_INCLUDE_DIRS})
    include(GoogleTest)

    target_link_libraries(test_lichens_cpp PRIVATE 
        lichens_cpp 
        GTest::gtest
        GTest::gtest_main
    )

    add_test(NAME TestLichensCppLibrary COMMAND test_lichens_cpp)
endif()
