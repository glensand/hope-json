cmake_minimum_required(VERSION 3.11)

project(hope-json)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add hope-core submodule (without tests to avoid googletest conflicts)
add_subdirectory(ext/hope-core/lib)

add_subdirectory(test)
add_subdirectory(samples/document)
add_subdirectory(lib)