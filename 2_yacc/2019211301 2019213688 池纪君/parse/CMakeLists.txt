cmake_minimum_required(VERSION 3.0)
project(parse) 

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB_RECURSE  SRC_LIST "src/*.cpp")
file(GLOB_RECURSE  TEST_LIST "test/*.cpp")


foreach(testfile IN LISTS TEST_LIST )
    get_filename_component(testname ${testfile} NAME_WE)
    add_executable(${testname}
        ${testfile}
        ${SRC_LIST}
    )
    
    
endforeach()
