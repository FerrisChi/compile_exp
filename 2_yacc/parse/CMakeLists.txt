cmake_minimum_required(VERSION 3.0)
project(parse) 

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB SRC_LIST "src/*.cpp")

add_executable(${CMAKE_PROJECT_NAME}
${SRC_LIST}
)