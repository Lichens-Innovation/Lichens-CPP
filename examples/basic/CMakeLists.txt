cmake_minimum_required(VERSION 3.14)

# Project
project(LichensCPPExampleBasic
    VERSION 1.0
    DESCRIPTION "Lichens C++ lib Demo Application"
    LANGUAGES CXX
)

include(${CMAKE_CURRENT_LIST_DIR}/lichens_cpp.cmake)

add_executable(ExampleBasic
    main.cpp
)
target_link_libraries(ExampleBasic PUBLIC 
    lichens_cpp
)

target_include_directories(ExampleBasic PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${lichens_cpp_SOURCE_DIR}/include
)
