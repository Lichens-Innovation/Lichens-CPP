# C++ standard used by the build recipes, e.g. `CMAKE_CXX_STANDARD=14 just test-all` or `just cxx_std=14 test-all` or ` just --set cxx_std 14 test-all`
cxx_std := env_var_or_default("CMAKE_CXX_STANDARD", "20")

help:
    cat README.md

open:
    code .

example-basic:
    cd examples/basic && mkdir -p build && cmake -S . -B build && cmake --build build && ./build/ExampleBasic

examples: example-basic


build:
    mkdir -p build/debug
    cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug -DBUILD_LICHENS_CPP_TESTS=OFF -DCMAKE_CXX_STANDARD={{cxx_std}} && cmake --build build/debug

build-test:
    mkdir -p build/test
    cmake -S . -B build/test -DCMAKE_BUILD_TYPE=Debug -DBUILD_LICHENS_CPP_TESTS=ON -DCMAKE_CXX_STANDARD={{cxx_std}} && cmake --build build/test

build-release:
    mkdir -p build/release
    cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release -DBUILD_LICHENS_CPP_TESTS=OFF -DCMAKE_CXX_STANDARD={{cxx_std}} && cmake --build build/release

test-ci: build-test
    ./build/test/test_lichens_cpp --gtest_output="json:TestsResults.json"

test-all: build-test
    ./build/test/test_lichens_cpp

test TEST_NAME: build-test
    ./build/test/test_lichens_cpp --gtest_filter={{TEST_NAME}}

clean:
    rm -rf build
    rm -rf examples/*/build