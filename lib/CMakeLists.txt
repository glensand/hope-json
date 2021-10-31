cmake_minimum_required(VERSION 3.15)
project(erocklib)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

file(GLOB R_HEADERS
    erock/*.h
    erock/strict/*.h
    erock/classic/*.h
)

add_library(erocklib INTERFACE ${R_HEADERS})
target_include_directories(erocklib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(erocklib 
    INTERFACE ${HOPE_INCLUDE_DIR}
    INTERFACE ${RAPIDJSON_INCLUDE_DIR}
)

add_dependencies(erocklib hope)
add_dependencies(erocklib rapidjson)

# instalation
set(CMAKE_INSTALL_DIRECTORY ${CMAKE_BINARY_DIR})
INSTALL(DIRECTORY erocklib/ DESTINATION ${CMAKE_INSTALL_DIRECTORY}/include/erocklib
        FILES_MATCHING PATTERN "*.h")
