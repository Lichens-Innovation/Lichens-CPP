cmake_minimum_required(VERSION 3.14)

# Project
project(LichensCPPExampleBasic
    VERSION 1.0
    DESCRIPTION "Lichens C++ lib Demo Application"
    LANGUAGES CXX
)

include(${CMAKE_CURRENT_LIST_DIR}/lichens_cpp.cmake)

option(EXAMPLE_BASIC_STRICT_WARNINGS "Build the example with strict consumer warning flags, warnings are errors." OFF)

add_executable(ExampleBasic
    main.cpp
    all_headers.cpp
)

# Strict flags a consumer project typically uses, only on the example target
if(EXAMPLE_BASIC_STRICT_WARNINGS)
    if(MSVC)
        target_compile_options(ExampleBasic PRIVATE /W4 /WX /permissive- /Zc:preprocessor /Zc:__cplusplus)
    else()
        target_compile_options(ExampleBasic PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()
endif()
target_link_libraries(ExampleBasic PUBLIC 
    lichens_cpp
)

target_include_directories(ExampleBasic PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${lichens_cpp_SOURCE_DIR}/include
)
