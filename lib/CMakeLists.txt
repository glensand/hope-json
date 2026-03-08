cmake_minimum_required(VERSION 3.15)
project(hopejsonlib)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

file(GLOB R_HEADERS
    hope/*.h
)

add_library(hopejsonlib INTERFACE ${R_HEADERS})
target_include_directories(hopejsonlib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(hopejsonlib INTERFACE hope-core)

# instalation
set(CMAKE_INSTALL_DIRECTORY ${CMAKE_BINARY_DIR})
INSTALL(DIRECTORY hope/ DESTINATION ${CMAKE_INSTALL_DIRECTORY}/include/hope
        FILES_MATCHING PATTERN "*.h")