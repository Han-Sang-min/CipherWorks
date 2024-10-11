cmake_minimum_required(VERSION 3.28)
project(testDLL)

set(VCPKG_PATH $ENV{vcpkg})
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "vcpkg toolchain")

set(CMAKE_CXX_STANDARD 17)

add_definitions(-D TESTDLL_EXPORTS)

add_library(testDLL SHARED library.cpp)

find_package(OpenSSL REQUIRED)
include_directories("${VCPKG_PATH}/installed/x64-windows/include")
include_directories("C:/Program Files/Java/jdk-23/include")
include_directories("C:/Program Files/Java/jdk-23/include/win32")
target_link_libraries(testDLL PRIVATE OpenSSL::SSL OpenSSL::Crypto)