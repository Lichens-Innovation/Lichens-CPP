help:
    cat README.md

open:
    code .

build:
    mkdir -p build/debug
    cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug -DBUILD_LICHENS_CPP_TESTS=OFF  && cmake --build build/debug

build-test:
    mkdir -p build/test
    cmake -S . -B build/test -DCMAKE_BUILD_TYPE=Debug -DBUILD_LICHENS_CPP_TESTS=ON && cmake --build build/test

build-release:
    mkdir -p build/release
    cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release -DBUILD_LICHENS_CPP_TESTS=OFF && cmake --build build/release

test-ci: build-test
    ./build/test/test_lichens_cpp --gtest_output="json:TestsResults.json"

test-all: build-test
    ./build/test/test_lichens_cpp

test TEST_NAME: build-test
    ./build/test/test_lichens_cpp --gtest_filter={{TEST_NAME}}

clean:
    rm -rf build