cmake_minimum_required(VERSION 3.24)
project(lichens_cpp_project LANGUAGES CXX)

cmake_policy(SET CMP0135 NEW)

# C++ 17 or better
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT DEFINED CMAKE_CXX_STANDARD OR CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "Lichens-CPP requires at least C++17. Current CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}")
endif()

message(STATUS "Lichens CPP using CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Warning and source encoding flags, applied only to Lichens-CPP's own targets.
# MSVC is checked first so clang-cl gets MSVC-style flags.
# Warnings are errors only when Lichens-CPP is the top-level project.
function(lichens_cpp_set_compile_options target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /W4 /we4101 /utf-8)
        if (lichens_cpp_project_IS_TOP_LEVEL)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        target_compile_options(${target} PRIVATE -Wall -Wextra -pedantic -Wuninitialized -Wunused-variable)
        if (lichens_cpp_project_IS_TOP_LEVEL)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    else()
        message(WARNING "Compiler not recognized, default warning flags will be used.")
    endif()
endfunction()

include(FetchContent)

# Library linkage. BUILD_SHARED_LIBS is intentionally ignored so that a consumer's
# global setting does not change how Lichens-CPP is built.
if(WIN32)
    set(LICHENS_CPP_BUILD_SHARED_DEFAULT OFF)
else()
    set(LICHENS_CPP_BUILD_SHARED_DEFAULT ON)
endif()
option(LICHENS_CPP_BUILD_SHARED "Build Lichens-CPP as a shared library (default: static on Windows, shared elsewhere)." ${LICHENS_CPP_BUILD_SHARED_DEFAULT})

# Fetched dependencies are always static, whatever the consumer's BUILD_SHARED_LIBS is.
# This normal variable only applies to this directory and the fetched subprojects.
set(BUILD_SHARED_LIBS OFF)

# CMake options to enable/disable modules
set(BUILD_LICHENS_CPP_LOG ON CACHE BOOL "Build the Log module.")
option(BUILD_LICHENS_CPP_CONFIG "Build the Config module." ON)
option(BUILD_LICHENS_CPP_LAUNCHER "Build the Launcher module." ON)
option(BUILD_LICHENS_CPP_MQTT "Build the MQTT module." ON)
option(BUILD_LICHENS_CPP_TESTS "Build the tests tree." ON)

set(SOURCES
    src/fileutil.cpp
    src/stringutil.cpp
    src/timeutil.cpp
    src/uuid.cpp
)

set(LIBRARY_USED
)

set(INCLUDES_PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set(INCLUDES_PRIVATE
)

set(MODULE_DEFINITIONS
)

# fmt and nlohmann_json: use an installed copy if found, otherwise fetch
FetchContent_Declare(
    fmt
    URL https://github.com/fmtlib/fmt/archive/refs/tags/11.1.4.tar.gz
    # spdlog 1.15 with an external fmt requires fmt 11 or newer
    FIND_PACKAGE_ARGS 11 GLOBAL
)
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
    FIND_PACKAGE_ARGS GLOBAL
)

# spdlog
if(BUILD_LICHENS_CPP_LOG)
    FetchContent_MakeAvailable(fmt)
    set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)
    set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "" FORCE)
    set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
      spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.3
    )
    FetchContent_MakeAvailable(spdlog)
    list(APPEND LIBRARY_USED
        fmt::fmt
        spdlog::spdlog
    )
    list(APPEND SOURCES 
        src/log/Logger.cpp
    )
    list(APPEND MODULE_DEFINITIONS BUILD_LICHENS_CPP_LOG)
endif()

if(BUILD_LICHENS_CPP_CONFIG)
    message(STATUS "Building Lichens-CPP Config module.")
    FetchContent_MakeAvailable(nlohmann_json)
    list(APPEND SOURCES 
        src/config/AppConfig.cpp
        src/config/Config.cpp
        src/config/ConfigValue.cpp
    )
    list(APPEND LIBRARY_USED
        nlohmann_json::nlohmann_json
    )
    list(APPEND MODULE_DEFINITIONS BUILD_LICHENS_CPP_CONFIG)
endif()

if(BUILD_LICHENS_CPP_LAUNCHER)
    message(STATUS "Building Lichens-CPP Launcher module.")
    list(APPEND SOURCES 
        src/launcher/MainLoopHelper.cpp
        src/launcher/PeriodicTask.cpp
    )
    # LaunchProcess uses POSIX process APIs
    if(WIN32)
        message(STATUS "Lichens-CPP LaunchProcess is not supported on Windows, it is excluded from the Launcher module.")
    else()
        list(APPEND SOURCES
            src/launcher/LaunchProcess.cpp
        )
    endif()
    list(APPEND MODULE_DEFINITIONS BUILD_LICHENS_CPP_LAUNCHER)
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
    list(APPEND MODULE_DEFINITIONS BUILD_LICHENS_CPP_MQTT)
endif()

# Library source files
if(LICHENS_CPP_BUILD_SHARED)
    set(LICHENS_CPP_LIBRARY_TYPE SHARED)
else()
    set(LICHENS_CPP_LIBRARY_TYPE STATIC)
endif()
add_library(lichens_cpp ${LICHENS_CPP_LIBRARY_TYPE}
    ${SOURCES}
)
# No export macros in the API, so export everything for a Windows DLL
set_target_properties(lichens_cpp PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

target_compile_features(lichens_cpp PUBLIC cxx_std_17)
target_compile_definitions(lichens_cpp PRIVATE ${MODULE_DEFINITIONS})
lichens_cpp_set_compile_options(lichens_cpp)

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
    message(STATUS "Building Lichens-CPP tests.")

    # GoogleTest dependencies
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(test_lichens_cpp
        tests/main.cpp
        tests/test_bimap.cpp
        tests/test_debouncing.cpp
        tests/test_ring_buffer.cpp
        tests/test_scope_action.cpp
        tests/test_stringutil.cpp
        tests/config/test_Config.cpp
        tests/config/test_ConfigValue.cpp
        tests/log/test_Logger.cpp
    )

    target_compile_definitions(test_lichens_cpp PRIVATE ${MODULE_DEFINITIONS} BUILD_LICHENS_CPP_TESTS)
    lichens_cpp_set_compile_options(test_lichens_cpp)

    include(GoogleTest)

    target_link_libraries(test_lichens_cpp PRIVATE 
        lichens_cpp 
        GTest::gtest
        GTest::gtest_main
    )

    add_test(NAME TestLichensCppLibrary COMMAND test_lichens_cpp)
endif()
