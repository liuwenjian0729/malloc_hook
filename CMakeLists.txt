cmake_minimum_required(VERSION 3.10.2)
project(malloc_hook)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_definitions(-D_GNU_SOURCE)

add_library(${PROJECT_NAME} SHARED malloc_hook.cc)
add_executable(${PROJECT_NAME}_main test_hook.cc)
target_link_libraries(${PROJECT_NAME} 
    dl
    pthread
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
