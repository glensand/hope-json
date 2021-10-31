project(document_sample)
set(BINARY document_sample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("../../lib")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB DOC_HEADERS
    *.h
)
    
file(GLOB DOC_SOURSES
    *.cpp
)

add_executable(${BINARY} ${DOC_HEADERS} ${DOC_SOURSES})

target_include_directories(${BINARY} 
  PUBLIC ${HOPE_INCLUDE_DIR}
  PUBLIC ${RAPIDJSON_INCLUDE_DIR}
)

add_dependencies(${BINARY} hope)
add_dependencies(${BINARY} rapidjson)
