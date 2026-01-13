if(TARGET lichens_cpp)
    return()
endif()

set(LICHENS_CPP_PATH "https://github.com/Lichens-Innovation/Lichens-CPP.git" CACHE STRING "Path to Lichens-CPP repository")
set(LICHENS_CPP_TAG "main" CACHE STRING "Tag to use for Lichens-CPP")

include(FetchContent)

# Select what we want to build from Lichens-CPP
set(BUILD_LICHENS_CPP_CONFIG OFF CACHE BOOL "Build Lichens-CPP configuration support" FORCE)
set(BUILD_LICHENS_CPP_LAUNCHER OFF CACHE BOOL "Build Lichens-CPP launcher" FORCE)
set(BUILD_LICHENS_CPP_MQTT OFF CACHE BOOL "Build Lichens-CPP MQTT support" FORCE)
set(BUILD_LICHENS_CPP_TESTS OFF CACHE BOOL "Build Lichens-CPP tests" FORCE)

FetchContent_Declare(
  lichens_cpp
  GIT_REPOSITORY "${LICHENS_CPP_PATH}"
  GIT_TAG "${LICHENS_CPP_TAG}"
  CMAKE_ARGS
    -DBUILD_LICHENS_CPP_CONFIG=${BUILD_LICHENS_CPP_CONFIG}
    -DBUILD_LICHENS_CPP_LAUNCHER=${BUILD_LICHENS_CPP_LAUNCHER}
    -DBUILD_LICHENS_CPP_MQTT=${BUILD_LICHENS_CPP_MQTT}
    -DBUILD_LICHENS_CPP_TESTS=${BUILD_LICHENS_CPP_TESTS}
)

FetchContent_MakeAvailable(lichens_cpp)

if(NOT EXISTS ${lichens_cpp_SOURCE_DIR})
  message(FATAL_ERROR "lichens_cpp source dir not found: ${lichens_cpp_SOURCE_DIR}")
endif()

if(NOT EXISTS ${lichens_cpp_SOURCE_DIR}/include)
  message(FATAL_ERROR "lichens_cpp include dir not found: ${lichens_cpp_SOURCE_DIR}/include")
endif()
