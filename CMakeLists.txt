cmake_minimum_required(VERSION 3.11)

project(erock-main)

enable_testing()

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
include("ext/hope.cmake")
include("ext/rapidjson.cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(test)
#add_subdirectory(samples/document)
add_subdirectory(lib)